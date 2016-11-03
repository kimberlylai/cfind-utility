/*CITS2002 Project 2 2016
 Names:			    Kimberly Lai Siru, Braden Ryan
 Student numbers:	21818156, 20930745
 Date:			    04-11-2016
 */

//This update fixed lots (16:10)
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

// Compile this program with:
// cc -std=c99 -Wall -Werror -pedantic -o cfind cfind.c

#define	OPTLIST		"acd:lrstu"

int main (int argc, char *argv[])
{
    
    if (argc==1){
        fprintf(stderr,"Error: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
        exit(EXIT_FAILURE); 		//exit indicating failure	
    }
    if (argc==2)
    {
        //check if file or directory
        //if yes, print the names of all files and directories at or below the provided pathname
        //if not, exit failure
        //check_type(argv[1]);
        //countfiles(argv[1]);
        //list_directory(argv[1]);
        //read(argv[1]);
        read_args(argc, argv);
        
    }
    if (argc > 2){
        //read_args(int argc, char *argv[]);
        //check_type(argv[1]);
        read_args(argc, argv);
    }
    
    
}

