/*CITS2002 Project 2 2016
 Names:			    Kimberly Lai Siru, Braden Ryan
 Student numbers:	21818156, 20930745
 Date:			    04-11-2016
 */

 // Compile this program with:
 // cc -std=c99 -Wall -Werror -pedantic -o cfind cfind.c

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include "statexpr.h"
#include "header.h"

#define	OPTLIST		"acd:lrstu"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		fprintf(stderr, "Error: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
		exit(EXIT_FAILURE); 		//exit indicating failure	
	}

	if (argc > 2)
	{
		read_args(argc, argv);
 }
}
