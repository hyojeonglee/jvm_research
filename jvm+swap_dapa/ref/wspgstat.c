#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include "pageidle.h"
#include "pfnofmap.h"

/*
 * Input: PID, delay, anon|[heap|stack]
 * Output 1: number of pages in working set
 * Output 2: working set size considering huge page
 */

typedef unsigned long long u8;

int nr_pfn;
int total_ws;
u8 total_wss;
int total_count;

int wspagesof(int pid, int delay, u8 pfns[], int is_huge[])
{
	int count = 0;
	u8 size = 0;

	time_t current_time;
	struct tm *d;
	int ms;
	struct timeval tv;
	char time_str[9];

	kill(pid, SIGSTOP);
	setidle(nr_pfn, pfns);
	kill(pid, SIGCONT);

	usleep(delay * 1000);

	kill(pid, SIGSTOP);
	count = nr_active(nr_pfn, pfns);
	size = sz_active(nr_pfn, pfns, is_huge);

	gettimeofday(&tv, NULL);
	ms = tv.tv_usec/1000;
	current_time = time(NULL);
	d = localtime(&current_time);

	strftime(time_str, sizeof(time_str), "%H:%M:%S", d);

	printf("%s.%3d | %10d\n", time_str, ms, count);
	printf("%s.%3d | %10llu\n\n", time_str, ms, size);
	kill(pid, SIGCONT);

	total_ws += count;
	total_wss += size;
	total_count++;

	return size;
}

void err_usage(const char *cmd)
{
	errx(1, "Usage: %s <PID> <delay> [mapping region]\n", cmd);
}

void sigint_handler(int signo)
{
	time_t current_time;
	struct tm *d;

	current_time = time(NULL);
	d = localtime(&current_time);
	printf("\nCURRENT TIME: %s", asctime(d));
	printf("AVERAGE # of WS: %d\n", (total_ws / total_count));
	printf("AVERAGE WSS: %llu\n", (total_wss / total_count));
	printf("Terminate by typing CTRL-C again.\n");
	signal(SIGINT, SIG_DFL);
}

struct arguments {
	int pid;
	int delay;
	char *mregions;
};

error_t parse_option(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key) {
	case ARGP_KEY_ARG:
		switch (state->arg_num) {
		case 0:
			arguments->pid = atoi(arg);
			break;
		case 1:
			arguments->delay = atoi(arg);
			break;
		case 2:
			arguments->mregions = arg;
			break;
		default:
			argp_usage(state);
		}
		break;
	case ARGP_KEY_END:
		/* At least two arguments are required */
		if (state->arg_num < 1)
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	time_t current_time;
	struct tm *d;

	struct argp argp = {
		.options = 0,
		.parser = parse_option,
		.args_doc = "<pid> <delay> [mapping regions]",
		.doc = "Periodically prints out number of working set pages"
	};

	struct arguments arguments;

	arguments.mregions = "[stack],[heap],anon";
	argp_parse(&argp, argc, argv, ARGP_IN_ORDER, NULL, &arguments);

	current_time = time(NULL);
	d = localtime(&current_time);
	printf("START TIME: %s", asctime(d));

	printf("Time	     |  WSSIZE\n");
	signal(SIGINT, sigint_handler);
	while (1) {
		nr_pfn = 0;
		int i = 0;
		struct PFN *pfn_head = malloc(sizeof(struct PFN));

		pfn_head->next = NULL;

		/*
		 * TODO: pfnofmap to pfnofmap_v2
		 * sj: Then, wspgstat doesn't works for now?
		 * pfn_head = pfnofmap(arguments.mregions, arguments.pid);
		 *
		 */
		if (pfn_head == NULL)
			err(2, "open maps");
		nr_pfn = getnum_pfn(pfn_head);
		u8 pfns[nr_pfn];

		int is_huge[nr_pfn];
		struct PFN *curr = pfn_head->next;

		while (curr != NULL) {
			pfns[nr_pfn - i - 1] = curr->val;
			if (curr->is_huge == 1)
				is_huge[nr_pfn - i - 1] = 1;
			i++;
			curr = curr->next;
		}
		wspagesof(arguments.pid, arguments.delay, pfns, is_huge);
	}

	return 0;
}
