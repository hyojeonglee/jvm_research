#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>

typedef unsigned long long u8;

struct VA {
	struct VA *next;
	u8 start;
	u8 end;
};

struct range {
	unsigned long start;
	unsigned long end;
};

void add_va(struct VA *target, u8 start, u8 end)
{
	struct VA *new = malloc(sizeof(struct VA));

	new->next = target->next;
	new->start = start;
	new->end = end;
	target->next = new;
}

struct PFN {
	struct PFN *next;
	u8 val;
	int is_huge;
};

void add_pfn(struct PFN *target, u8 val, int is_huge)
{
	struct PFN *new = malloc(sizeof(struct PFN));

	new->next = target->next;
	new->val = val;
	new->is_huge = is_huge;
	target->next = new;
}

int getnum_pfn(struct PFN *target)
{
	int num = 0;
	struct PFN *curr = target->next;

	while (curr != NULL) {
		num++;
		curr = curr->next;
	}

	return num;
}

static char *map_field(char *line)
{
	int i;
	int skipped_words = 0;

	for (i = 0; i < strlen(line) - 1; i++) {
		if (line[i] == ' ' && line[i + 1] != ' ') {
			/* sixth word is mapping file field */
			if (skipped_words++ == 4)
				return &line[i + 1];
		}
	}

	return NULL;
}

/* Compare two strings but ignore final new line */
static int strcmp_n(const char *str1, const char *str2)
{
	for (; *str1 && (*str1 == *str2); str1++, str2++)
		;

	if (*str1 == '\n' || *str2 == '\n')
		return 0;

	return *str1 - *str2;
}

/*
 * Extract virtual address range from a line which is read from
 * /proc/<pid>/maps file if the range is mapped to one of target mapping files
 * or regions.
 *
 * @maps: array of target mapping files or regions
 * @nr_maps: number of targets in @maps
 * @line: a line which is read from /proc/<pid>/maps file
 * @range: pointer of struct range that result will be saved
 *
 * Returns zero if success, non-zero otherwise.
 */
int va_for_map(char **maps, int nr_maps, char *line, struct range *range)
{
	int i;
	char *map;
	char *cursor;

	map = map_field(line);

	for (i = 0; i < nr_maps; i++) {
		if (map == NULL) {
			if (strcmp(maps[i], "anon") == 0)
				break;
			continue;
		}
		if (strcmp_n(maps[i], map) == 0)
			break;
	}
	if (i == nr_maps)
		return -1;

	/* First word is the range in form of <start>-<end> */
	range->start = strtol(line, NULL, 16);
	cursor = strchr(line, '-');
	range->end = strtol(cursor + 1, NULL, 16);

	return 0;
}

/*
 * Get virtual address range of specific mapping regions
 *
 * @pid		process id in string form
 * @regions	array of target regions (e.g., [stack], [heap], anon)
 * @nr_regions	number of regions in @regions array
 *
 * Parse /proc/<pid>/maps file and get virtual address ranges for target
 * regions.  The regions can be [stack], [heap], the file name for file-backed
 * pages, or anon for anonymous pages that allocated with mmap().
 *
 * Returns pointer to head VA node if success, NULL if failed
 */
struct VA *vaof(int pid, char **regions, int nr_regions)
{
	struct VA *head;
	FILE *fp;
	char *line;
	size_t len = 0;
	char maps_path[20]; /* Enough for 10-digit pid */

	sprintf(maps_path, "/proc/%d/maps", pid);
	fp = fopen(maps_path, "r");

	/*
	 * TODO: Add documents
	 * When using -g option, it cause unexpected exit of program
	 */
	if (fp == NULL)
		return NULL;

	head = malloc(sizeof(struct VA));
	head->next = NULL;
	while (getline(&line, &len, fp) != -1) {
		struct range range;

		if (va_for_map(regions, nr_regions, line, &range))
			continue;
		add_va(head, range.start, range.end);
	}

	fclose(fp);

	struct VA *curr = head->next->next;
	struct VA *collapsed_head = malloc(sizeof(struct VA));
	struct VA *collapsed_curr;

	collapsed_head->next = NULL;
	add_va(collapsed_head, head->next->start, head->next->end);
	collapsed_curr = collapsed_head->next;

	while (curr != NULL) {
		if (collapsed_curr->end == curr->start)
			collapsed_curr->end = curr->end;
		else
			add_va(collapsed_head, curr->start, curr->end);
		curr = curr->next;
	}

	return collapsed_head;
}

/*
 * Convert comma seperated string string into array of strings
 * TODO: Ref astr func at misc.c
 */
int parse_regions(char *mregion, char ***regionsptr)
{
	char **regions;
	int nr_regions = 1;
	char *check;
	char *ptr;
	int i;

	if (strcmp(mregion, "") == 0)
		return 0;

	regions = (char **)malloc(nr_regions * sizeof(char *));

	check = strchr(mregion, ',');
	if (check == NULL)
		regions[0] = mregion;
	else {
		while (check != NULL) {
			check = strchr(check + 1, ',');
			/* sj: Shouldn't regions be allocated again? */
			nr_regions++;
		}

		char mre_tmp[25];

		strcpy(mre_tmp, mregion);

		ptr = strtok(mre_tmp, ",");
		i = 0;
		while (ptr != NULL) {
			/*
			 * sj: mre_tmp is being accessed from out of its scope.
			 * Is this safe?
			 *
			 * To figure out, I checked heap region change before
			 * and after the malloc() call for `regions` using
			 * /proc/`pidof test_units`/maps.  The range for heap
			 * was to 01587000-015a8000 and the malloc() call
			 * returned 0x1587830.  IOW, it was safe to use
			 * 21,120,394 bytes (0x15a8000 - 0x1587830).
			 *
			 * That said, the code is insafe, obviously.  This bug
			 * should be fixed.
			 */
			regions[i] = ptr;
			ptr = strtok(NULL, ",");
			i++;
		}
	}

	*regionsptr = regions;
	return nr_regions;
}

u8 PAGEMAP_PFN(u8 ent)
{
	return (ent & ((1ull << 55) - 1));
}

int IN_LRU(u8 kpflags)
{
	if ((kpflags & (1ull << 5)) != 0)
		return 1;
	return 0;
}

int IS_HUGE(u8 kpflags)
{
	if ((kpflags & (1ull << 17)) != 0)
		return 1;
	return 0;
}

#define BASE_PAGE_SHIFT	12
#define BASE_PAGE_SIZE (1 << 12)
#define HUGE_PAGE_SIZE (1 << 21)

/* variable length array */
struct pfnarr {
	u8 *array;
	char *is_huge;
	size_t nr_entries;
	size_t capacity;
};

/*
 * Returns 0 for success, non-zero else.
 */
int pfnarr_append(struct pfnarr *a, u8 pfn, char is_huge)
{
	if (a->nr_entries == a->capacity) {
		a->capacity *= 2;
		a->array = realloc(a->array, sizeof(u8) * a->capacity);
		if (a->array == NULL)
			return -1;
		a->is_huge = realloc(a->is_huge, sizeof(char) * a->capacity);
		if (a->is_huge == NULL)
			return -1;

	}
	a->array[a->nr_entries] = pfn;
	a->is_huge[a->nr_entries++] = is_huge;

	return 0;
}

void pfnarr_init(struct pfnarr *a, size_t cap)
{
	a->array = (u8 *)malloc(sizeof(u8) * cap);
	a->is_huge = (char *)malloc(sizeof(char) * cap);
	a->nr_entries = 0;
	a->capacity = cap;
}

/*
 * Get pfns of a process for specific regions
 *
 * @mregion	memory regions string in csv format
 * @pid		target process id
 * @pfns	savepoint of pfn array
 * @huge_bitmap	savepoint of huge page bitmap
 *
 * This function is version two of pfnofmap.  Main difference is that this
 * version returns array of pfn instead of the PFN list.  In future, this will
 * substitute the first version.
 *
 * If @huge_bitmap is NULL, it will not be saved.
 *
 * Returns number of pfns for success, negative number for failure.
 */
int pfnofmap_v2(char *mregion, int pid, u8 **pfns, char **huge_bitmap)
{
	int nr_regions = 1;
	char **regions = NULL;

	struct VA *vas_head, *curr;
	char pmap_path[25];	/* enough for 10-digit pid */
	char kpflg_path[] = "/proc/kpageflags";
	int f, kpflgf;

	struct pfnarr arr;

	nr_regions = parse_regions(mregion, &regions);

	/* Read virtual address ranges of the process for the target regions */
	vas_head = vaof(pid, regions, nr_regions);
	/* TODO: Add documents */
	if (vas_head == NULL)
		return -1;
	free(regions);

	sprintf(pmap_path, "/proc/%d/pagemap", pid);
	f = open(pmap_path, O_RDONLY);
	kpflgf = open(kpflg_path, O_RDONLY);

	pfnarr_init(&arr, 256);
	for (curr = vas_head->next; curr != NULL; curr = curr->next) {
		u8 start_va = curr->start;
		u8 end_va = curr->end;
		u8 vaddr;

		/* Get pfns of a virtual address range */
		for (vaddr = start_va; vaddr <= end_va;
				vaddr += BASE_PAGE_SIZE) {
			u8 ent, pfn = 0;
			u8 kpflags;
			char is_huge = 0;

			u8 offset = vaddr >> (BASE_PAGE_SHIFT - 3);

			/* Get pfn of the virtual address */
			lseek(f, offset, SEEK_SET);
			if (read(f, &ent, 8) == 8) {
				pfn = PAGEMAP_PFN(ent);
				if (pfn == 0)
					continue;
			} else {
				/* TODO: Add documents */
				return -1;
			}

			/* Handle properties of the page */
			/* TODO: Add documents */
			lseek(kpflgf, pfn * 8, SEEK_SET);
			if (read(kpflgf, &kpflags, 8) == 8) {
				if (IN_LRU(kpflags) == 0)
					continue;
				if (IS_HUGE(kpflags) == 1) {
					is_huge = 1;
					vaddr += HUGE_PAGE_SIZE -
						BASE_PAGE_SIZE;
				}
			} else {
				err(2, "%s: read kpageflag", __func__);
			}
			pfnarr_append(&arr, pfn, is_huge);
		}
	}
	close(f);
	close(kpflgf);

	*pfns = arr.array;
	if (huge_bitmap != NULL) {
		*huge_bitmap = arr.is_huge;
		free(arr.is_huge);
	}

	return arr.nr_entries;
}
