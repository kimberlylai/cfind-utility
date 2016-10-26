/*
 CITS2002 Project 2 2016
 Names:			    Kimberly Lai Siru, Braden Ryan
 Student numbers:	21818156, 20930745
 Date:			    04-11-2016
 
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <errno.h>

#define	OPTLIST		"acd:lrstu"

static void searchforfilename(char argv) {
  for (int i = 0; i < ?; i++) {
    if (strcmp(argv, dirp[i].name) == 0) {
      printf
    }
  }

void list_directory(char *dirname)
{
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(dirname);
    if(dirp == NULL)
    {
    perror( progname );
    exit(EXIT_FAILURE);
    }
    while((dp = readdir(dirp)) != NULL)
    {
    printf( "%s\n", dp->d_name );
    }
    closedir(dirp);
}

void count(char *dirname)
{
    int filecount = 0;
    DIR *dirp;
    struct dirent *entry;
    
    dirp = opendir(dirname);
    if(dirp == NULL)
    {
    perror( progname );
    exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dirp)) != NULL) {
    if (entry->d_type == DT_REG || entry->d_type == DT_DIR) { /* If the entry is a regular file */
         filecount++;  
    }
    printf("%d", filecount);
}
closedir(dirp);

int read_args(int argc, char *argv[]){
    int  opt;
    bool aflag   = false; //all entries be considered, including file-entries beginning with the '.' character
    bool cflag   = false; //Print only the count of the number of matching file-entries, then exit
    bool lflag   = false; //print long listing (permissions, inode, number of links, owner's name, group-owner's name, size, modificate-date, and name)
    bool rflag   = false; //Reverse the order of any sorting options.
    bool sflag   = false; //Print matching file-entries, sorted by size. If both -s and -t are provided, -t takes precedence.
    bool tflag   = false; //Print matching file-entries, sorted by modification time. If both -s and -t are provided, -t takes precedence.
    bool uflag   = false; //Attempt to unlink (remove) as many matching file-entries as possible. The cfind utility should exit with failure if any attempt to unlink a file-entry was unsuccessful.
    char *filenm = NULL;
    int  depth   = DEFAULT_VALUE; //Limit the search to the indicated depth, descending at most depth levels

  
    while((opt = getopt(argc, argv, OPTLIST)) != -1) {
        //  ACCEPT A BOOLEAN ARGUMENT
        if(opt == 'a')
        {
            aflag  =  !aflag;
            read_all(argv[1]);
        }
        else if(opt == 'c')
        {
            cflag  =  !cflag;
        }
        else if(opt == 'r')
        {
            rflag  =  !rflag;
        }
        else if(opt == 's')
        {
            sflag  =  !sflag;
        }
        else if(opt == 't')
        {
            tflag  =  !tflag;
        }
        else if(opt == 'l')
        {
            lflag  =  !lflag;
        }
        else if(opt == 'u')
        {
            uflag  =  !uflag;
        }
        //  ACCEPT A INTEGER ARGUMENT
        else if(opt == 'd')
        {
            depth  =  atoi(optarg);
        }
        //  OOPS - AN UNKNOWN ARGUMENT
        else 
        {
            fprintf(stderr,"Error: %s \nusage: ./cfind [options]  pathname  [stat-expression]\n", strerror(errnum));

        }
    }
    
    
    if(argc <= 0) {    //  display program's usage/help
        usage(1);
    }

    return 0;
}
int read(char *pathname){
    //do something;
    
    return 0;
}
int read_all(char *pathname){
    return 0;
}
int count(char *pathname){
    return 0;
}
int depth(char *pathname, int value){
    return 0;
}
int long_list(char *pathname){
    return 0;
}
int reverse_sort(char *pathname){
    return 0;
}
int size_sort(char *pathname){

    return 0;
}
int time_sort(char *pathname){
    return 0;
}
int unlink(char *pathname){
    return 0;
}
int check_type(char *pathname){
    //checks if pathname specified is a file or a directories
    //use dirent DTYPE
    //if file return 1
    //if dir return 2
    struct stat statbuf;
    if(stat(pathname, &s)==0){
        if (statbuf.st_mode & S_IFDIR) //if it is a dir
        {
            return 2;
        }
        else if (statbuf.st_mode & S_IFREG) //if it's a file
        {
            return 1;
        }
        else 
        {
            printf("Error: \"%s\" is neither a file nor a directory. Try again. \n", pathname);
            exit(EXIT_FAILURE); //it's neither
        }
    }
    printf("Error: \"%s\" is neither a file nor a directory. Try again. \n", pathname);
    exit(EXIT_FAILURE); 
    
}
int main (int argc, char *argv[])
{
    
    if (argc==1){
        fprintf(stderr,"Error: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errnum));
        exit(EXIT_FAILURE); 		//exit indicating failure	
    }
    if (argc==2){
        //check if file or directory
        //if yes, print the names of all files and directories at or below the provided pathname
        //if not, exit failure
        check_type(argv[1]);
        read(argv[1]);
    }
    if (argc > 2){
        read_args(int argc, char *argv[]);
    }
    	
}
