/*
 * Declarations and definitions that doesn't intended to be exported, but
 * intended to be tested comes here.
 */

#ifndef _TEST_UNITS_H

struct range {
	unsigned long start;
	unsigned long end;
};

int va_for_map(char **maps, int nr_maps, char *line, struct range *range);

int parse_regions(char *mregion, char ***regionsptr);

#endif
