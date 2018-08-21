#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pageidle.h"

#define BIT_AT(val, x)	(((val) & (1ull << x)) != 0)
#define SET_BIT(val, x) ((val) | (1ull << x))

#define HUGE_PAGE_SIZE 2048000
#define BASE_PAGE_SIZE 4096

typedef unsigned long long u8;

int open_bitmap(int flags)
{
	int fd;

	fd = open("/sys/kernel/mm/page_idle/bitmap", flags);
	if (fd < 0)
		err(2, "Can't open bitmap. Is CONFIG_IDLE_PAGE_TRACKING off?");

	return fd;
}

/*
 * Mark specfied page frames as idle
 */
void setidle(int nr_pfns, u8 pfns[])
{
	int fd;
	u8 pfn, entry;
	int i;

	fd = open_bitmap(O_RDWR);

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), pfn / 64 * 8)
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		entry = SET_BIT(entry, pfn % 64);
		if (pwrite(fd, &entry, sizeof(entry), pfn / 64 * 8)
				!= sizeof(entry))
			err(2, "%s: write bitmap", __func__);
	}
	close(fd);
}

/*
 * pfn to index of idle page file bitmap
 *
 * The bitmap should be read in 8 bytes (64 pages) stride.
 */
#define PFN_TO_IPF_IDX(pfn) (pfn >> 6 << 3)

/*
 * Read and print out idle bits of specified page frames
 */
int *getidle(int nr_pfns, u8 pfns[])
{
	int fd;
	u8 entry, pfn;
	int i;
	/* sj: Is the memory be free at last? */
	/* hj: It'll be freed at ... */
	int *idleness = (int *)malloc(nr_pfns * sizeof(int));

	fd = open_bitmap(O_RDONLY);

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), PFN_TO_IPF_IDX(pfn))
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		idleness[i] = (int)BIT_AT(entry, pfn % 64);
	}

	close(fd);

	return idleness;
}

/*
 * Read idle bits of specified page frames and count number of zeroed bits
 */
u8 nr_active(int nr_pfns, u8 pfns[])
{
	int fd;
	u8 entry, pfn;
	int i;
	u8 nr_activepages = 0;

	fd = open_bitmap(O_RDONLY);

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry), PFN_TO_IPF_IDX(pfn))
				!= sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		/* Modification: Get all used pages */
		if (!BIT_AT(entry, pfn % 64))
			nr_activepages++;
	}
	close(fd);

	return nr_activepages;
}

u8 sz_active(int nr_pfns, u8 pfns[], int is_huge[])
{
	int fd;
	u8 entry, pfn;
	int i;
	int flg_huge = 0;
	u8 sz_activepages = 0;

	fd = open_bitmap(O_RDONLY);

	for (i = 0; i < nr_pfns; i++) {
		pfn = pfns[i];
		flg_huge = is_huge[i];
		entry = 0;
		if (pread(fd, &entry, sizeof(entry),
					PFN_TO_IPF_IDX(pfn)) != sizeof(entry))
			err(2, "%s: read bitmap", __func__);
		if (!BIT_AT(entry, pfn % 64)) {
			if (flg_huge == 1)
				sz_activepages += HUGE_PAGE_SIZE;
			else
				sz_activepages += BASE_PAGE_SIZE;
		}
	}
	close(fd);

	return sz_activepages;
}
