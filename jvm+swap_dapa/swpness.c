#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

typedef unsigned long long u8;

struct VA {
	struct VA *next;
	u8 start;
	u8 end;
};

void add_va(struct VA *target, u8 start, u8 end)
{
	struct VA *new = malloc(sizeof(struct VA));

	new->next = target->next;
	new->start = start;
	new->end = end;
	target->next = new;
}

/*
 * ref: https://www.kernel.org/doc/Documentation/vm/pagemap.txt
 *
 * [I/O related page flags]
 * 13. SWAPCACHE   page is mapped to swap space, ie. has an associated swap
 *     entry
 * 14. SWAPBACKED  page is backed by swap/RAM
 *
 * So, We use flag number 14.
 *
 */

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

int IS_SWP(u8 kpflags)
{
	if ((kpflags & (1ull << 14)) != 0)
		return 1;
	return 0;
}

// TODO: useless.
struct VA *get_vas(void)
{
	struct VA *head;

	return head;
}

#define BASE_PAGE_SHIFT 12
#define BASE_PAGE_SIZE (1 << 12)
#define HUGE_PAGE_SIZE (1 << 21)

// TODO: how to get pid of jvm summary process?
// First, we will use pidof or jps for test.
int cal_swpness(int pid, u8 beg, u8 end)
{
	struct VA *vas_head, *curr;
	char pmap_path[25];	/* enough for 10-digit pid */
	char kpflg_path[] = "/proc/kpageflags";
	int pmapf, kpflgf;
	int tot_cnt = 0;
	int swp_cnt = 0;

	// TODO: useless.
	// how to get vaddrs of java object in region?
	// vas_head = get_vas();
	// TODO: error handling

	sprintf(pmap_path, "/proc/%d/pagemap", pid);
	pmapf = open(pmap_path, O_RDONLY);
	kpflgf = open(kpflg_path, O_RDONLY);

	for (curr = vas_head->next; curr != NULL; curr = curr->next) {
		u8 start_va = curr->start;
		u8 end_va = curr->end;
		u8 vaddr;

		for (vaddr = start_va; vaddr < end_va;
				vaddr += BASE_PAGE_SIZE) {
			u8 ent, pfn = 0;
			u8 kpflags;

			u8 offset = vaddr >> (BASE_PAGE_SHIFT - 3);

			lseek(pmapf, offset, SEEK_SET);
			if (read(pmapf, &ent, 8) == 8) {
				pfn = PAGEMAP_PFN(ent);
				if (pfn == 0)
					continue;
			} else {
				// TODO: error handling
			}

			lseek(kpgflgf, pfn * 8, SEEK_SET);
			if (read(kpflgf, &kpflags, 8) == 8) {
				if (IN_LRU(kpflags) == 0)
					continue;
				if (IS_HUGE(kpflags) == 1) {
					vaddr += HUGE_PAGE_SIZE -
						BASE_PAGE_SIZE;
				}
				// Increase swp obj counter
				if (IS_SWP(kpflags) == 1)
					swp_cnt++;
				// Increase total counter
				tot_cnt++;
			} else {
				err(2, "%s: read kpageflag", __func__);
			}
		}
	}
	close(pmapf);
	close(kpflgf);

	// Summarize swapness (swapped pages / total pages in LRU list)
	printf("Swapped pages / Total pages: %d\n", int(swp_cnt / tot_cnt));

	return 0;
}

