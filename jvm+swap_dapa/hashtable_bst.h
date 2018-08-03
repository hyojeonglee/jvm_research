typedef unsigned long long u8;

#define HASH_MAX 1000

struct NODE {
	u8 pfn;
	int pg_access;
	int old_pg_access;
	struct NODE *left;
	struct NODE *right;
};

struct BST {
	struct NODE *head;
};

struct BST *hash_arr;

void hash_add(u8 pfn, int pg_access);
void hash_display(void);
void hash_init(void);
void hash_save_result(char *file_path, int gap_save);
