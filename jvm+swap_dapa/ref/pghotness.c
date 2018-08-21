#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <err.h>
#include <sys/stat.h>

#include "pageidle.h"
#include "pfnofmap.h"
#include "hashtable_bst.h"
#include "misc.h"

/* Optional values */
int pid;
char *mregion = "[stack],[heap],anon";
char *file_path = "final_output";
int interval;
int nomsg;
int gap_save;

void sigint_handler(int signo)
{
	time_t cur_time;
	struct tm *d;

	cur_time = time(NULL);
	d = localtime(&cur_time);
	if (!nomsg)
		printf("CURRENT TIME: %s", asctime(d));

	if (!nomsg)
		hash_display();

	signal(SIGINT, SIG_DFL);
}

void save_result(void)
{
	if (!nomsg) {
		printf("Workload %d has been terminated.\n", pid);
		printf("The result will be saved in file: %s.\n", file_path);
	}
	if (!nomsg)
		hash_display();
	hash_save_result(file_path, gap_save);
}

void save_gap_result(void)
{
	/* sj: aclk_clock() returns `unsigned`. */
	int cur_time;

	/* sj: CLOCKS_PER_SEC is for clock().  We should use real cpu freq */
	cur_time = (int) aclk_clock() / (CLOCKS_PER_SEC / 1000);
	if (cur_time % gap_save == 0) {
		char *gap_path = "./masim_test/checkpoint";

		if (!nomsg)
			printf("Checkpoint to %s.\n", gap_path);
		hash_save_result(gap_path, gap_save);
	}
}

error_t parse_option(int key, char *arg, struct argp_state *state)
{
	switch (key) {
	case ARGP_KEY_ARG:
		switch (state->arg_num) {
		case 0:
			pid = atoi(arg);
			break;
		case 1:
			interval = atoi(arg);
			break;
		default:
			argp_usage(state);
		}
		break;
	case 'r':
		mregion = arg;
		break;
	case 'f':
		file_path = (char *)malloc(sizeof(char) * strlen(arg) + 1);
		strcpy(file_path, arg);
		break;
	case 's':
		nomsg = 1;
		break;
	case 'g':
		gap_save = atoi(arg);
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 2) /* At least 2 argument */
			argp_usage(state);
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp_option options[] = {
	{
		.name = "mregion",
		.key = 'r',
		.arg = "<memory regions>",
		.flags = 0,
		.doc = "List of target memory regions.",
		.group = 0,
	},
	{
		.name = "file_path",
		.key = 'f',
		.arg = "<path of result file>",
		.flags = 0,
		.doc = "Path of a file that result will be saved into.",
		.group = 0,
	},
	{
		.name = "no-message",
		.key = 's',
		.arg = 0,
		.flags = 0,
		.doc = "Suppress stdout message.",
		.group = 0,
	},
	{
		.name = "save_gap",
		.key = 'g',
		.arg = "<gap of save point>",
		.flags = 0,
		.doc = "Gap of save point. (ms)",
		.group = 0,
	},
	{}
};

void pr_starttime(void)
{
	time_t cur_time;
	struct tm *d;

	if (nomsg)
		return;
	cur_time = time(NULL);
	d = localtime(&cur_time);
	printf("START TIME: %s", asctime(d));
}

void pr_time(void)
{
	time_t cur_time;
	struct tm *d;
	char time_str[9];

	if (nomsg)
		return;
	cur_time = time(NULL);
	d = localtime(&cur_time);

	strftime(time_str, sizeof(time_str), "%H:%M:%S", d);
	printf("%s\n", time_str);
}

void validate_pid(int pid)
{
	struct stat sts;
	char proc_path[15];

	sprintf(proc_path, "/proc/%d", pid);
	if (stat(proc_path, &sts) == -1 && errno == ENOENT)
		err(2, "There is no process with pid %d", pid);
}

int *alloc_pgaccess(int *pg_access, int nr_pfns, int nr_pfns_old)
{
	if (pg_access == NULL) {
		/* sj: When this be free?  Maybe memory leak? */
		pg_access = (int *)calloc(nr_pfns, sizeof(int));
	} else if (nr_pfns != nr_pfns_old) {
		pg_access = (int *)realloc(pg_access, nr_pfns * sizeof(int));

		if (nr_pfns > nr_pfns_old)
			memset(&pg_access[nr_pfns_old], 0,
					(nr_pfns - nr_pfns_old) * sizeof(int));
	}

	return pg_access;
}

int main(int argc, char **argv)
{
	int *idleness;
	int nr_pfn, nr_pfn_old;
	int *pg_access = NULL;
	u8 *pfns = NULL;
	int i;

	struct argp argp = {
		.options = options,
		.parser = parse_option,
		.args_doc = "<pid> <interval>",
		.doc = "Page hotness analyzer",
	};

	argp_parse(&argp, argc, argv, ARGP_IN_ORDER, NULL, NULL);

	if (!nomsg)
		printf("Pid: %d\nmregion: %s\nfile path: %s\n"
				"interval(ms): %d\nsave gap: %d\n",
				pid, mregion, file_path, interval, gap_save);

	pr_starttime();

	hash_arr = (struct BST *)malloc(HASH_MAX * sizeof(struct BST *));
	hash_init();

	signal(SIGINT, sigint_handler);

	nr_pfn_old = 0;

	validate_pid(pid);

	while (1) {
		if (pfns != NULL)
			free(pfns);

		nr_pfn = pfnofmap_v2(mregion, pid, &pfns, NULL);
		if (nr_pfn < 0) {
			/*
			 * TODO: Add documents to pfnofmap
			 */
			save_result();
			return 0;
		}

		pg_access = alloc_pgaccess(pg_access, nr_pfn, nr_pfn_old);
		nr_pfn_old = nr_pfn;
		kill(pid, SIGSTOP);
		setidle(nr_pfn, pfns);
		kill(pid, SIGCONT);
		usleep(interval * 1000);
		kill(pid, SIGSTOP);
		/* sj: Shouldn't we free idleness?  Maybe memory leak? */
		idleness = getidle(nr_pfn, pfns);

		/*
		 * sj: If the hash table supports 'increment' operation, which
		 * increments value of the pair for given key, we can remove
		 * pg_access.  It would be more simple and efficient.
		 */
		for (i = 0; i < nr_pfn; i++) {
			if (idleness[i] == 0)
				pg_access[i]++;
		}

		for (i = 0; i < nr_pfn; i++)
			hash_add(pfns[i], pg_access[i]);

		kill(pid, SIGCONT);

		if (gap_save != 0)
			save_gap_result();
	}
	if (pfns != NULL)
		free(pfns);

	return 0;
}
