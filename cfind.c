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
#include <sys/stat.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define	OPTLIST		"acd:lrstu"

/**static void searchforfilename(char argv) {
 for (int i = 0; i < ?; i++) {
 if (strcmp(argv, dirp[i].name) == 0) {
 printf
 }
 }**/
int check_type(char *pathname){
        //checks if pathname specified is a file or a directories
        //use dirent DTYPE
        //if file return 1
        //if dir return 2
        struct stat statbuf;
        if(stat(pathname, &statbuf)==0){
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
void list_directory(char *dirname)
{
    DIR *dirp;
    struct dirent *dp;
    char* path = dirname;
    size_t pathlen = strlen(dirname);
    dirp = opendir(dirname);
    if(dirp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    while((dp = readdir(dirp)) != NULL)
    {
        char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
        if (fullpath == NULL) { 
        /* deal with error and exit */ 
        printf("Error");
        }
        //printf("%s/%s\n",path, ep->d_name);
        if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))==0){
            //printf("%s\n",dp->d_name);
            continue;
        }else{
        sprintf(fullpath, "%s/%s", path, dp->d_name);
        puts(fullpath);
        if ((check_type(fullpath))==2){
            list_directory(fullpath);
        }
        free(fullpath);
        }
                    
    }
    closedir(dirp);
}
void countfiles2(char *dirname)
{
    DIR *dirp;
    struct dirent *dp;
    char* path = dirname;
    size_t pathlen = strlen(dirname);
    dirp = opendir(dirname);
    int filecount=0;
    if(dirp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    while((dp = readdir(dirp)) != NULL)
    {
        char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
        if (fullpath == NULL) {
            /* deal with error and exit */
            printf("Error");
        }
        //printf("%s/%s\n",path, ep->d_name);
        if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))==0){
            //printf("%s\n",dp->d_name);
            continue;
        }else{
            sprintf(fullpath, "%s/%s", path, dp->d_name);
            //puts(fullpath);
            
            //filecount++;
            if ((check_type(fullpath))==2){
                filecount++;
                countfiles2(fullpath);
            }
            else if ((check_type(fullpath))==1){
                filecount++;
            }
            free(fullpath);
        }
        
    }
    printf("here!!! %d\n",filecount);
    closedir(dirp);
}

    int read_args(int argc, char *argv[]){
        int  opt;
        bool aflag   = false; //all entries be considered, including file-entries beginning with the '.' character
        bool cflag   = false; //Print only the count of the number of matching file-entries, then exit
        bool lflag   = false; //print long listing (permissions, inode, number of links, owner's name, group-owner's name, size, modificate-date, and name)
        bool rflag   = false; //Reverse the order of any sorting options.
        bool sflag   = false; //Print matching file-entries, sorted by size. If both -s and -t are provided, -t takes precedence.
        bool tflag   = false; //Print matching file-entries, sorted by modification time. If both -s and -t are provided, -t takes precedence.
        bool uflag   = false; //Attempt to unlink (remove) as many matching file-entries as possible. The cfind utility should exit with failure if any attempt to unlink a file-entry was unsuccessful.
        //char *filenm = NULL;
        int  depth   = 0; //Limit the search to the indicated depth, descending at most depth levels
        
        
        while((opt = getopt(argc, argv, OPTLIST)) != -1) {
            //  ACCEPT A BOOLEAN ARGUMENT
            if(opt == 'a')
            {
                aflag  =  !aflag;
                //read_all(argv[1]);
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
                fprintf(stderr,"Error: %s \nusage: ./cfind [options]  pathname  [stat-expression]\n", strerror(errno));
                
            }
        }
        
        
        if(argc <= 0) {    //  display program's usage/help
            fprintf(stderr,"Error: %s \nusage: ./cfind [options]  pathname  [stat-expression]\n", strerror(errno));
        }
        
        return 0;
    }
    int read_file(char *pathname){
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
    int unlink_file(char *pathname){
        return 0;
    }
    
    int main (int argc, char *argv[])
    {
        
        if (argc==1){
            fprintf(stderr,"Error: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
            exit(EXIT_FAILURE); 		//exit indicating failure	
        }
        if (argc==2){
            //check if file or directory
            //if yes, print the names of all files and directories at or below the provided pathname
            //if not, exit failure
            check_type(argv[1]);
            countfiles2(argv[1]);
            //list_directory(argv[1]);
            //read(argv[1]);
        }
        if (argc > 2){
            //read_args(int argc, char *argv[]);
            check_type(argv[1]);
        }
        
        
    }
