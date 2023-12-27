
#include <string>
#include <vector>
#include <iostream>

#include "mathparser.h"

//https://github.com/ccxvii/asstools/blob/master/getopt.c
/*
 * This is a version of the public domain getopt implementation by
 * Henry Spencer originally posted to net.sources.
 *
 * This file is in the public domain.
 */

#include <string.h>
#define getopt xgetopt
#define optarg xoptarg
#define optind xoptind

char *optarg; /* Global argument pointer. */
int optind = 0; /* Global argv index. */

static char *scan = NULL; /* Private scan pointer. */

int
getopt(int argc, char *argv[], char *optstring)
{
	char c;
	char *place;

	optarg = NULL;

	if (!scan || *scan == '\0') {
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
			return EOF;
		if (argv[optind][1] == '-' && argv[optind][2] == '\0') {
			optind++;
			return EOF;
		}

		scan = argv[optind]+1;
		optind++;
	}

	c = *scan++;
	place = strchr(optstring, c);

	if (!place || c == ':') {
		fprintf(stderr, "%s: unknown option -%c\n", argv[0], c);
		return '?';
	}

	place++;
	if (*place == ':') {
		if (*scan != '\0') {
			optarg = scan;
			scan = NULL;
		} else if( optind < argc ) {
			optarg = argv[optind];
			optind++;
		} else {
			fprintf(stderr, "%s: option requires argument -%c\n", argv[0], c);
			return ':';
		}
	}

	return c;
}

//end getopt.c

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "need an expression..." << std::endl;
		exit(1);
	}
	
	bool graph = false;
	
	int f;
	while ((f = getopt(argc, argv, (char *) "g")) != -1)
		switch(f) {
			case 'g':  //Read input file's rawproc toolchain, edit it according to the specified changes, and process accordingly
				graph = true;
				break;
		}
	
	std::string s = std::string(argv[optind]);

	Parser p;
	float answer;
	
	if (graph) {
		p.parse(s, answer);
		std::cout << p.graphWriteGraph();
	}
	else {
		if (p.parse(s, answer)) {
			std::cout << answer << std::endl;
		}
		else {
			std::cout << p.getError() << std::endl;
		}
	}
	

}