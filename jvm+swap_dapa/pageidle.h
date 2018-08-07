typedef unsigned long long u8;

void setidle(int nr_pfns, u8 pfns[]);
int *getidle(int nr_pfns, u8 pfns[]);
u8 nr_active(int nr_pfns, u8 pfns[]);
u8 sz_active(int nr_pfn, u8 pfns[], int is_huge[]);
