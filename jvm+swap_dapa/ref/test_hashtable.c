#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include "hashtable_bst.h"

u8 *create_pfn(int len, int limit)
{
	u8 i;
	u8 *pfn = (u8 *)malloc(sizeof(u8) * len);

	srand(time(NULL));
	int check_dup[limit];

	for (i = 0; i < limit; i++)
		check_dup[i] = 0;

	for (i = 0; i < len; )  {
		u8 temp = rand() % limit;

		if (check_dup[temp] == 0) {
			check_dup[temp] = 1;
			pfn[i] = temp + 1;
			i++;
		}
	}

	return pfn;
}

int test_hash(int len, int limit)
{
	int i;
	/* It would be better to be more generalized */
	u8 *pfn = create_pfn(len, limit);
	int fail = 0;
	int res = 0;

	for (i = 0; i < len; i++)
		hash_add(pfn[i], i);

	for (i = 0; i < len; i++) {
		/*
		 * TODO: Modify hashtable func
		 * int res = hash_get_val(pfn[i]);
		 */

		/* It would be better to just print FAIL and exit */
		if (i != res)
			fail++;
	}

	printf("Result(fail/total): %d / %d\n", fail, len);

	return 0;
}

int main(void)
{
	hash_arr = (struct BST *)malloc(HASH_MAX * sizeof(struct BST *));
	hash_init();

	/* test case */
	/*
	 * sj: Maybe we need more test case for scenarios like below:
	 * - Inserting duplicated key objects
	 * - Searching for objects that is not stored before
	 * - Deleting objects
	 * - Deleting objects that is not stored before
	 * - etc...
	 * Could be better to unify with test_units?
	 */
	test_hash(10, 1000);
	test_hash(1000, 100000);
	test_hash(100000, 1000000);
	test_hash(1000000, 1000000);

	return 0;
}
