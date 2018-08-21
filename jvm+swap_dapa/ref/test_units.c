#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test_units.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define pr_fail_msg(...)						\
	do {								\
		printf("%s[fail]%s %s: %s(line %d)\n",			\
				KRED, KYEL,				\
				__FILE__, __func__, __LINE__);		\
		printf("\t");						\
		printf(__VA_ARGS__);					\
		printf("%s", KNRM);					\
	} while (0)

#define ates_fail_if(cond, ...)						\
	do {								\
		if (!(cond))						\
			break;						\
		printf("%s[fail]%s %s: %s(line %d)\n",			\
				KRED, KYEL,				\
				__FILE__, __func__, __LINE__);		\
		printf("\t");						\
		printf(__VA_ARGS__);					\
		printf("%s", KNRM);					\
		exit(1);						\
	} while (0)

int test_vaof_line(void)
{
	struct range range;
	char *maps[3];
	int result;
	static char *const lines[] = {
		"7f9ae9556000-7f9ae9557000 rw-p 00000000 00:00 0\n",
		"7ffc44272000-7ffc44293000 rw-p 00000000 00:00 0    [stack]\n",
		"7ffc443a4000-7ffc443a7000 r--p 00000000 00:00 0    [vvar]\n",
	};

	maps[0] = "anon";
	result = va_for_map(maps, 1, lines[0], &range);
	ates_fail_if(result != 0, "Fails for right input\n");
	ates_fail_if(range.start != 0x7f9ae9556000 ||
			range.end != 0x7f9ae9557000, "Wrong address\n");

	result = va_for_map(maps, 1, lines[1], &range);
	ates_fail_if(result == 0, "Success for wrong input\n");

	maps[0] = "[stack]";
	result = va_for_map(maps, 1, lines[1], &range);
	ates_fail_if(result != 0, "Fail for right input\n");
	ates_fail_if(range.start != 0x7ffc44272000 ||
			range.end != 0x7ffc44293000, "Wrong address\n");

	/* multiple maps */
	maps[0] = "anon", maps[1] = "[stack]";
	result = va_for_map(maps, 2, lines[0], &range);
	ates_fail_if(result != 0, "Fails for right input\n");
	ates_fail_if(range.start != 0x7f9ae9556000 ||
			range.end != 0x7f9ae9557000, "End address wrong\n");

	result = va_for_map(maps, 2, lines[2], &range);
		ates_fail_if(result == 0, "Success for wrong input\n");

	result = va_for_map(maps, 2, lines[1], &range);
	ates_fail_if(result != 0, "Fails for right input\n");
	ates_fail_if(range.start != 0x7ffc44272000 ||
			range.end != 0x7ffc44293000, "Wrong address\n");

	return 0;
}

int test_parse_regions(void)
{
	char **regions;
	int nr_regions;

	nr_regions = parse_regions("", &regions);
	ates_fail_if(nr_regions != 0, "Empty string but nr_regions %d\n",
			nr_regions);

	nr_regions = parse_regions("foo", &regions);
	ates_fail_if(nr_regions != 1, "input 'foo' but nr_regions %d\n",
			nr_regions);
	ates_fail_if(strcmp("foo", regions[0]) != 0,
			"input 'foo' but parsed region %s\n", regions[0]);

	nr_regions = parse_regions("foo,bar", &regions);
	ates_fail_if(nr_regions != 2, "input 'foo,bar' but nr_regions %d\n",
			nr_regions);

	nr_regions = parse_regions("foo,bar,baz", &regions);
	ates_fail_if(nr_regions != 3,
			"input 'foo,bar,baz' but nr_regions %d\n", nr_regions);

	return 0;
}

int (*tests[])(void) = {test_vaof_line, test_parse_regions};

int main(void)
{
	int i;

	for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
		if (tests[i]()) {
			printf("%sFAIL%s\n", KRED, KNRM);
			return 1;
		}
	}

	printf("%sPASS%s\n", KGRN, KNRM);
	return 0;
}
