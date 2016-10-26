#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <getopt.h>
#include <statexpr.h> 

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
    if (entry->d_type == DT_REG) { /* If the entry is a regular file */
         filecount++;  
    }
    printf("%d", filecount);
}
closedir(dirp);

#define	OPTLIST		"acd:lrstu"
int read_args(int argc, char *argv[]){
    int  opt;
    bool aflag   = false;
    bool cflag   = false;
    bool lflag   = false;
    bool rflag   = false;
    bool sflag   = false;
    bool tflag   = false;
    bool uflag   = false;
    char *filenm = NULL;
    int  depth   = DEFAULT_VALUE;
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
            count(char *argv)
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
            printf("error? no such argument exists.")
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
    
}
int main (int argc, char *argv[])
{
    
    if (argc==1){
        printf("error! did not specify filename \nusage: ./cfind [file system pathname] [conditional expressions]\n");
        exit(EXIT_FAILURE); 		//exit indicating failure	
    }
    if (argc==2){
        //check if file or directory
        //if yes, print the names of all files and directories at or below the provided pathname
        //if not, exit failure
        read(argv[1]);
    }
    if (argc > 2){
        read_args(int argc, char *argv[]);
    }
    	
}
