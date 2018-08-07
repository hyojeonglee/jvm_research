#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "hashtable_bst.h"

/*
 * Very specialized for number of accesses of pfn case.  It would be better to
 * be generalized or have more specialized name for less confusion.
 *
 * No delete operation?
 */

int SIZE;

/* for save result */
int g_index;

static struct NODE *find(struct NODE *tree, u8 pfn);
static void insert_element(struct NODE *tree, struct NODE *item);
static void display_tree(struct NODE *tree);
static void save_result(struct NODE *tree, FILE *fp, int gap_save);

static int hashcode(u8 pfn)
{
	return (int)(pfn % (u8)HASH_MAX);
}

void hash_add(u8 pfn, int pg_access)
{
	int index = hashcode(pfn);
	struct NODE *tree = (struct NODE *)hash_arr[index].head;
	struct NODE *new_item = (struct NODE *)malloc(sizeof(struct NODE));

	new_item->pfn = pfn;
	new_item->pg_access = pg_access;
	new_item->old_pg_access = 0;
	new_item->left = NULL;
	new_item->right = NULL;

	if (tree == NULL) {
		hash_arr[index].head = new_item;
		SIZE++;
	} else {
		struct NODE *tmp = find(tree, pfn);

		if (tmp == NULL) {
			insert_element(tree, new_item);
			SIZE++;
		} else {
			tmp->pg_access = pg_access;
		}
	}
}

static struct NODE *find(struct NODE *tree, u8 pfn)
{
	if (tree == NULL)
		return NULL;
	if (tree->pfn == pfn)
		return tree;
	else if (pfn < tree->pfn)
		return find(tree->left, pfn);
	else
		return find(tree->right, pfn);
}

static void insert_element(struct NODE *tree, struct NODE *item)
{
	if (item->pfn < tree->pfn) {
		if (tree->left == NULL) {
			tree->left = item;
			return;
		}
		insert_element(tree->left, item);
		return;
	} else if (item->pfn > tree->pfn) {
		if (tree->right == NULL) {
			tree->right = item;
			return;
		}
		insert_element(tree->right, item);
		return;
	}
}

void hash_display(void)
{
	int i;

	for (i = 0; i < HASH_MAX; i++) {
		struct NODE *tree = hash_arr[i].head;

		if (tree != NULL)
			display_tree(tree);
	}
}

static void display_tree(struct NODE *tree)
{
	if (tree == NULL)
		return;
	printf("[%llu] %d\n", tree->pfn, tree->pg_access);

	if (tree->left != NULL)
		display_tree(tree->left);
	if (tree->right != NULL)
		display_tree(tree->right);
}

void hash_init(void)
{
	int i = 0;

	for (i = 0; i < HASH_MAX; i++)
		hash_arr[i].head = NULL;
}

int size_of_hashtable(void)
{
	return SIZE;
}

static void save_result(struct NODE *tree, FILE *fp, int gap_save)
{
	char index[100], access[100];
	char *line = NULL;
	int tmp_acc;

	if (tree) {
		sprintf(index, "%d", g_index + 1);
		g_index++;
		if (gap_save == 0) {
			tmp_acc = tree->pg_access;
		} else {
			tmp_acc = tree->pg_access - tree->old_pg_access;
			tree->old_pg_access = tree->pg_access;
		}
		sprintf(access, "%d", tmp_acc);
		line = (char *)malloc((strlen(index) +
					strlen(access) + 2) * sizeof(char));
		sprintf(line, "%s %s\n", index, access);
		fwrite(line, strlen(line), 1, fp);
		save_result(tree->left, fp, gap_save);
		save_result(tree->right, fp, gap_save);
	}
}

void hash_save_result(char *file_path, int gap_save)
{
	int i;
	FILE *fp;

	fp = fopen(file_path, "a");
	if (fp == NULL)
		err(2, "open result file");
	fwrite("X Y\n", 4, 1, fp);
	for (i = 0; i < HASH_MAX; i++) {
		struct NODE *tree = hash_arr[i].head;

		if (tree != NULL)
			save_result(tree, fp, gap_save);
	}
	g_index = 0;
	fclose(fp);
}
