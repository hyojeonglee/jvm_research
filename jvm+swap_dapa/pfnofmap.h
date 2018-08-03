typedef unsigned long long u8;

struct PFN {
	struct PFN *next;
	u8 val;
	int is_huge;
};

struct PFN *pfnofmap(char *mregion, int pid);
int getnum_pfn(struct PFN *target);
int pfnofmap_v2(char *mregion, int pid, u8 **pfns, char **huge_bitmap);
