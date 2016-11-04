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

#define	OPTLIST		"acd:lrstu"

bool check_type(char *pathname) 
{
	//checks if pathname specified is a file or a directories
	//use dirent DTYPE
	//if it is a file, return 1
	//if it is a dir, return 0
	struct stat statbuf;
	if (stat(pathname, &statbuf) == 0)
	{
		if (statbuf.st_mode & S_IFDIR) //if it is a dir
		{
			return 0;
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

void sort_struct_time(struct dirent **namelist, int left, int right) 
{
	//Sorts stat structure by last modified time
	int sortleft;
	int sortright;

	struct dirent *temp;
	struct stat i_buffer;
	struct stat j_buffer;
	struct stat x_buffer;
	sortleft = left; sortright = right;

	stat(namelist[sortleft]->d_name, &i_buffer);
	stat(namelist[sortright]->d_name, &j_buffer);
	stat(namelist[(left + right) / 2]->d_name, &x_buffer);

	do {
		while ((i_buffer.st_mtime < x_buffer.st_mtime) && (sortleft < right))
		{
			sortleft++;
			stat(namelist[sortleft]->d_name, &i_buffer);
		}
		while ((j_buffer.st_mtime > x_buffer.st_mtime) && (sortright > left))
		{
			sortright--;
			stat(namelist[sortright]->d_name, &j_buffer);
		}
		if (sortleft <= sortright) 
		{
			temp = namelist[sortleft];
			namelist[sortleft] = namelist[sortright];
			namelist[sortright] = temp;
			sortleft++; sortright--;
			stat(namelist[sortleft]->d_name, &i_buffer);
			stat(namelist[sortright]->d_name, &j_buffer);
		}
	} 
	while (sortleft <= sortright);
	if (left < sortright) sort_struct_time(namelist, left, sortright);
	if (sortleft < right) sort_struct_time(namelist, sortleft, right);
}

void sort_struct_size(struct dirent **namelist, int left, int right) 
{
	//Sorts stat structure by size
	int sortleft;
	int sortright;

	struct dirent *temp;
	struct stat i_buffer;
	struct stat j_buffer;
	struct stat x_buffer;
	sortleft = left; sortright = right;

	stat(namelist[sortleft]->d_name, &i_buffer);
	stat(namelist[sortright]->d_name, &j_buffer);
	stat(namelist[(left + right) / 2]->d_name, &x_buffer);

	do {
		while ((i_buffer.st_size < x_buffer.st_size) && (sortleft < right))
		{
			sortleft++;
			stat(namelist[sortleft]->d_name, &i_buffer);
		}
		while ((j_buffer.st_size > x_buffer.st_size) && (sortright > left))
		{
			sortright--;
			stat(namelist[sortright]->d_name, &j_buffer);
		}
		if (sortleft <= sortright) 
		{
			temp = namelist[sortleft];
			namelist[sortleft] = namelist[sortright];
			namelist[sortright] = temp;
			sortleft++; sortright--;
			stat(namelist[sortleft]->d_name, &i_buffer);
			stat(namelist[sortright]->d_name, &j_buffer);
		}
	} 
	while (sortleft <= sortright);
	if (left < sortright) sort_struct_size(namelist, left, sortright);
	if (sortleft < right) sort_struct_size(namelist, sortleft, right);
}

void quick_struct(struct dirent **namelist, int count, bool tflag, bool sflag)
{
	if(tflag==1 && sflag ==0)
	{
		sort_struct_time(namelist, 0, count - 1);
	}
	else if (tflag ==0 && sflag == 1)
	{
		sort_struct_size(namelist, 0, count - 1);
	}
	else 
	{
		sort_struct_time(namelist, 0, count - 1);
		sort_struct_size(namelist, 0, count - 1);
	}
}

void sort_time(char *pathname, bool tflag, bool sflag, bool aflag, bool lflag){
		struct dirent **namelist;
		int n;
		const char *targetdirectory = pathname;
		char *path = pathname;
		struct tm   *tm;
		char datestring[256];
		size_t pathlen = strlen(pathname);
		n = scandir(targetdirectory, &namelist, 0, alphasort);
		struct stat statbuf;
		if (n < 0)
		{
			fprintf(stderr, "Error: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
		}
		else 
		{
			{
				quick_struct(namelist, n,tflag,sflag);
				while (n--)
				{
					if ((strcmp(namelist[n]->d_name, ".") && strcmp(namelist[n]->d_name, "..")) == 0)
					{
						//ignore . & ..
						continue;
					}
					if (aflag == 0){
						if (namelist[n]->d_name[0] == '.')
						{
							continue;
						}
					}
				
					stat(namelist[n]->d_name, &statbuf);
					char *fullpath = malloc(pathlen + strlen(namelist[n]->d_name) + 2);
					tm = localtime(&statbuf.st_mtime);
					strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
					sprintf(fullpath, "%s/%s", path,namelist[n]->d_name);
					puts(fullpath);
					
					if ((check_type(fullpath)) == 0) 
					{
						sort_time(fullpath, tflag, sflag, aflag, lflag);
					}
					free(namelist[n]);
					free(fullpath);
					
					
				}
				free(namelist);
			}
		}
}

void sort_size(char *pathname, bool tflag, bool sflag, bool aflag, bool lflag){
		struct dirent **namelist;
		int n;
		const char *targetdirectory = pathname;
		char *path = pathname;
		size_t pathlen = strlen(pathname);
		n = scandir(targetdirectory, &namelist, 0, alphasort);
		struct stat statbuf;
		if (n < 0)
		{
			fprintf(stderr, "Error: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
		}
		else 
		{
			{
				quick_struct(namelist, n,tflag,sflag);
				while (n--)
				{
					if ((strcmp(namelist[n]->d_name, ".") && strcmp(namelist[n]->d_name, "..")) == 0)
					{
						//ignore . & ..
						continue;
					}
					if (aflag == 0){
						if (namelist[n]->d_name[0] == '.')
						{
							continue;
						}
					}
					stat(namelist[n]->d_name, &statbuf);
					char *fullpath = malloc(pathlen + strlen(namelist[n]->d_name) + 2);
					sprintf(fullpath, "%s/%s", path,namelist[n]->d_name);
					puts(fullpath);
					if ((check_type(fullpath)) == 0) 
					{
						sort_size(fullpath, tflag, sflag, aflag, lflag);
					}
					free(namelist[n]);
					free(fullpath);
					
				}
				free(namelist);
			}
		}
	
}
void list_directory_stat_exp(char *dirname, bool aflag, bool lflag, STAT_EXPRESSION statexpr)
{
	//prints matching directories and files to a stat expression
	DIR *dirp;
	struct dirent *dp;
	struct stat statbuf;
	char *path = dirname;
	size_t pathlen = strlen(dirname);
	dirp = opendir(dirname);
	struct passwd *pwd;
	struct group *grp;
	struct tm   *tm;
	char datestring[256];

	if (dirp == NULL)
	{
		//If directory is empty exit
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		const char *name = dp->d_name;
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);

		if (fullpath == NULL)
		{
			//if NULL. print error and exit
			fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			//Ignore . and ..
			continue;
		}

		else {
			sprintf(fullpath, "%s/%s", path, dp->d_name);
			if (stat(fullpath, &statbuf) != 0)
			{
				continue;
			}
			else if (evaluate_stat_expression(statexpr, name, &statbuf) == 1)
			{
				if (aflag == 1) 
				{
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if (lflag == 1)
					{
						if (stat(fullpath, &statbuf) == 0)
						{
							printf("%llu\t", statbuf.st_ino); //Print inode
							printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
							printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
							printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
							printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
							printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
							printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
							printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
							printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
							printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
							printf((statbuf.st_mode & S_IXOTH) ? "x" : "-"); //Print permissions
							printf("\t");
							printf("%hu\t", statbuf.st_nlink); //Print links
							if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
								printf("%s\t", pwd->pw_name); //Print owner
							if ((grp = getgrgid(statbuf.st_gid)) != NULL)
								printf("%s\t", grp->gr_name);//Print group
							printf("%jd\t", (intmax_t)statbuf.st_size); //Print size
							tm = localtime(&statbuf.st_mtime);//Get localized date string
							strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
							printf(" %s\t", datestring);//Print date
						}
					}
					puts(fullpath);
				}
				else 
				{
					if (dp->d_name[0] == '.')
					{
						continue;
					}
					else 
					{
						sprintf(fullpath, "%s/%s", path, dp->d_name);
						if (lflag == 1)
						{
							if (stat(fullpath, &statbuf) != -1)
							{
								printf("%llu\t", statbuf.st_ino); //Print inode
								printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
								printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
								printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
								printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
								printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
								printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
								printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
								printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
								printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
								printf((statbuf.st_mode & S_IXOTH) ? "x" : "-"); //Print permissions
								printf("\t");
								printf("%hu\t", statbuf.st_nlink); //Print links
								if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
									printf("%s\t", pwd->pw_name); //Print owner
								if ((grp = getgrgid(statbuf.st_gid)) != NULL)
									printf("%s\t", grp->gr_name);//Print group
								printf("%jd\t", (intmax_t)statbuf.st_size); //Print size
								tm = localtime(&statbuf.st_mtime);//Get localized date string
								strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
								printf(" %s\t", datestring);//Print date
							}
						}
						puts(fullpath);
					}
				}
				if ((check_type(fullpath)) == 0) 
				{
					list_directory_stat_exp(fullpath, aflag, lflag, statexpr);
				}
			}
			free(fullpath);
		}

	}
	closedir(dirp);
}

void list_directory(char *dirname, bool aflag, bool lflag)
{
	//Prints directory when a stat expression is not specified
	DIR *dirp;
	struct dirent *dp;
	struct stat statbuf;
	char *path = dirname;
	size_t pathlen = strlen(dirname);
	dirp = opendir(dirname);
	struct passwd *pwd;
	struct group *grp;
	struct tm   *tm;
	char datestring[256];

	if (dirp == NULL)
	{
		//If directory is empty exit
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL)
		{
			//if NULL. print error and exit//if NULL. print error and exit
			fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			//Ignore . and ..
			continue;
		}
		else 
		{
			if (aflag == 1)
			{
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				if (lflag == 1)
				{
					if (stat(fullpath, &statbuf) == 0)
					{
						printf("%llu\t", statbuf.st_ino); //Print inode
						printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
						printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
						printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
						printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
						printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
						printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
						printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
						printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
						printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
						printf((statbuf.st_mode & S_IXOTH) ? "x" : "-"); //Print permissions
						printf("\t");
						printf("%hu\t", statbuf.st_nlink); //Print links
						if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
							printf("%s\t", pwd->pw_name); //Print owner
						if ((grp = getgrgid(statbuf.st_gid)) != NULL)
							printf("%s\t", grp->gr_name);//Print group
						printf("%jd\t", (intmax_t)statbuf.st_size); //Print size
						tm = localtime(&statbuf.st_mtime);//Get localized date string
						strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
						printf(" %s\t", datestring);//Print date
					}
				}
				puts(fullpath);
			}
			else 
			{
				if (dp->d_name[0] == '.')
				{
					continue;
				}
				else
				{
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if (lflag == 1)
					{
						if (stat(fullpath, &statbuf) != -1)
						{
							printf("%llu\t", statbuf.st_ino); //Print inode
							printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
							printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
							printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
							printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
							printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
							printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
							printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
							printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
							printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
							printf((statbuf.st_mode & S_IXOTH) ? "x" : "-"); //Print permissions
							printf("\t");
							printf("%hu\t", statbuf.st_nlink); //Print links
							if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
								printf("%s\t", pwd->pw_name); //Print owner
							if ((grp = getgrgid(statbuf.st_gid)) != NULL)
								printf("%s\t", grp->gr_name);//Print group
							printf("%jd\t", (intmax_t)statbuf.st_size); //Print size
							tm = localtime(&statbuf.st_mtime);//Get localized date string
							strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
							printf(" %s\t", datestring);//Print date
						}
					}
					puts(fullpath);
				}
			}
			if ((check_type(fullpath)) == 0) 
			{
				list_directory(fullpath, aflag, lflag);
			}
			free(fullpath);
		}
	}
	closedir(dirp);
}

void list_directory_depth(char *dirname, bool aflag, bool lflag, int depth)
{
	//Prints directory when the depth option is used
	DIR *dirp;
	struct dirent *dp;
	char *path = dirname;
	size_t pathlen = strlen(dirname);
	dirp = opendir(dirname);

	if (dirp == NULL)
	{
		//If directory is empty exit
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL && depth >= 0)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL) {
			//if NULL. print error and exit//if NULL. print error and exit
			fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
			
		}
		//printf("%s/%s\n",path, ep->d_name);
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			//Ignore . and ..
			continue;
		}
		else
		{
			if (dp->d_name[0] == '.')
			{
				//Ignore hidden files
				continue;
			}
			else 
			{
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				puts(fullpath);
			}

			if ((check_type(fullpath)) == 0 && depth > 0)
			{
				list_directory(fullpath, aflag, lflag);
				depth--;
			}
			free(fullpath);
		}
	}
	closedir(dirp);
}

int count_all(char *dirname, bool aflag, int filecount)
{
	//Prints the number of directories and files
	DIR *dirp;
	struct dirent *dp;
	char *path = dirname;
	size_t pathlen = strlen(dirname);
	dirp = opendir(dirname);

	if (dirp == NULL)
	{
		//If directory is empty exit
		exit(EXIT_FAILURE);
	}

	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL) {
			//if NULL. print error and exit//if NULL. print error and exit
			fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
			
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			//Ignore . and ..
			continue;
		}
		else
		{
			if (aflag == 1) 
			{
				filecount++;
				sprintf(fullpath, "%s/%s", path, dp->d_name);
			}
			else 
			{
				if (dp->d_name[0] == '.') 
				{
					continue;
				}
				else 
				{
					filecount++;
					sprintf(fullpath, "%s/%s", path, dp->d_name);
				}
			}
			if ((check_type(fullpath)) == 0) 
			{
				filecount = count_all(fullpath, aflag, filecount);
			}
			free(fullpath);
		}
	}
	closedir(dirp);
	return filecount;
}

void unlink_dir(char *pathname, bool aflag) 
{
	//Unlinks specified files and directories
	DIR *dirp;
	struct dirent *dp;
	char *path = pathname;
	size_t pathlen = strlen(pathname);
	dirp = opendir(pathname);

	if (dirp == NULL)
	{
		//If directory is empty exit
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL)
		{
			//if NULL. print error and exit//if NULL. print error and exit
			fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
			
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			//Ignore . and ..
			continue;
		}
		else 
		{
			if (aflag == 1) 
			{
				//Remove hidden files
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				if ((check_type(fullpath)) == 1)
				{
					//Remove the files
					unlink(fullpath);
				}
				else if ((check_type(fullpath)) == 0) 
				{
					//Remove the (now empty) directory
					unlink_dir(fullpath, aflag);
					rmdir(fullpath); //Remove the (now empty) directory
				}
			}
			else
			{
				if (dp->d_name[0] == '.')
				{
					//Ingore hidden files
					continue;
				}
				else {
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if ((check_type(fullpath)) == 1) 
					{
						//Remove the files
						unlink(fullpath);
					}
					else if ((check_type(fullpath)) == 0) 
					{
						//Remove the (now empty) directory
						unlink_dir(fullpath, aflag);
						if (rmdir(fullpath) == -1) 
						{
							fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
						}
						rmdir(fullpath); //Remove the (now empty) directory
					}
				}
			}
		}
		free(fullpath);
	}
	closedir(dirp);
}

void unlink_dir_depth(char *pathname, bool aflag, bool dflag, int depth) 
{
	//Unlink files and directories when the depth option is used
	DIR *dirp;
	struct dirent *dp;
	char *path = pathname;
	size_t pathlen = strlen(pathname);
	dirp = opendir(pathname);
	if (dirp == NULL)
	{
		//If directory is empty exit
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL && depth >= 0)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL) 
		{
			//if NULL. print error and exit//if NULL. print error and exit
			fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
			
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0) 
		{
			//Ignore . and ..
			continue;
		}
		else
		{
			if (aflag == 1) 
			{
				//Remove hidden files
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				if (check_type(fullpath) == 1 && depth > 0)
				{
					unlink(fullpath);
				}
				else if (check_type(fullpath) == 1 && depth == 0)
				{
					unlink(fullpath);
				}
				else if ((check_type(fullpath)) == 0 && depth > 0) 
				{
					rmdir(fullpath); //Remove the (now empty) directory
					if (rmdir(fullpath) == -1)
					{
						fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
					}
					depth--;
					unlink_dir_depth(fullpath, aflag, dflag, depth);
				}
				else if (check_type(fullpath) == 0 && depth == 0)
				{
					if (rmdir(fullpath) == -1)
					{
						fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
					}
					rmdir(fullpath); //Remove the (now empty) directory
				}
			}
			else 
			{
				if (dp->d_name[0] == '.') 
				{
					//Ingore hidden files
					continue;
				}
				else 
				{
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if (check_type(fullpath) == 1 && depth > 0) 
					{
						unlink(fullpath);
					}
					else if (check_type(fullpath) == 1 && depth == 0) 
					{
						unlink(fullpath);
					}
					else if ((check_type(fullpath)) == 0 && depth > 0)
					{
						rmdir(fullpath); //remove the (now empty) directory
						if (rmdir(fullpath) == -1) 
						{
							fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
						}
						depth--;
						unlink_dir_depth(fullpath, aflag, dflag, depth);
					}
					else if (check_type(fullpath) == 0 && depth == 0) 
					{
						if (rmdir(fullpath) == -1) 
						{
							fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
						}
						rmdir(fullpath); //remove the (now empty) directory
					}
				}
			}
			free(fullpath);
		}
	}
	closedir(dirp);
}

int read_args(int argc, char *argv[]) 
{
	//Reads and parses the arguments inputted
	int  opt;
	bool aflag = false; //All entries be considered, including file-entries beginning with the '.' character
	bool cflag = false; //Print only the count of the number of matching file-entries, then exit
	bool dflag = false; //Print directories to a spceified depth
	bool lflag = false; //Print long listing (permissions, inode, number of links, owner's name, group-owner's name, size, modificate-date, and name)
	bool rflag = false; //Reverse the order of any sorting options.
	bool sflag = false; //Print matching file-entries, sorted by size. If both -s and -t are provided, -t takes precedence.
	bool tflag = false; //Print matching file-entries, sorted by modification time. If both -s and -t are provided, -t takes precedence.
	bool uflag = false; //Attempt to unlink (remove) as many matching file-entries as possible. The cfind utility should exit with failure if any attempt to unlink a file-entry was unsuccessful.
	int  depth = 0; //Limit the search to the indicated depth, descending at most depth levels
	int filecount = 0;
	
	while ((opt = getopt(argc, argv, OPTLIST)) != -1) 
	{
		//  ACCEPT A BOOLEAN ARGUMENT
		if (opt == 'a')
		{
			aflag = !aflag;
		}
		else if (opt == 'c')
		{
			cflag = !cflag;
		}
		else if (opt == 'r')
		{
			rflag = !rflag;
		}
		else if (opt == 's')
		{
			sflag = !sflag;
		}
		else if (opt == 't')
		{
			tflag = !tflag;
		}
		else if (opt == 'l')
		{
			lflag = !lflag;
		}
		else if (opt == 'u')
		{
			uflag = !uflag;
		}
		//  ACCEPT A INTEGER ARGUMENT
		else if (opt == 'd')
		{
			dflag = !dflag;
			depth = atoi(optarg);
		}
		//  OOPS - AN UNKNOWN ARGUMENT
		else
		{
			fprintf(stderr, "Error: %s \nusage: ./cfind [options]  pathname  [stat-expression]\n", strerror(errno));
			exit(EXIT_FAILURE); 		//exit indicating failure
		}
	}
	

	if (argc <= 0) 
	{
		//Display error and usage
		fprintf(stderr, "Error: %s \nusage: ./cfind [options]  pathname  [stat-expression]\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (access(argv[optind], F_OK) != 0)
	{
		//check if file or directory exists, if not display error and usage
		fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", argv[optind], strerror(errno));
		exit(EXIT_FAILURE); 		//exit indicating failure
	}

	if (cflag == 1) {
		//if pointing to a file, count as 1
		if((check_type(argv[optind]))==1){
			printf("1\n");
			exit(EXIT_SUCCESS);
		}else{
		printf("%d\n", count_all(argv[optind], aflag, filecount));
		exit(EXIT_SUCCESS);
		}
	}
	if (sflag == 1)
	{
		sort_size(argv[optind], tflag, sflag, aflag, lflag);
		exit(EXIT_SUCCESS);
	}
	if (tflag == 1)
	{
		sort_time(argv[optind], tflag, sflag, aflag, lflag);
		exit(EXIT_SUCCESS);
	}
	if (uflag == 1 && dflag == 0)
	{
		if ((check_type(argv[optind])) == 1) //if it is a file, unlink the file
		{
			unlink((argv[optind]));
			exit(EXIT_SUCCESS);
		}
		else if ((check_type(argv[optind])) == 0) //else if it's a directory, run our function
		{
			unlink_dir(argv[optind], aflag);
			exit(EXIT_SUCCESS);
		}
	}
	if (uflag == 1 && dflag == 1)
	{
		if ((check_type(argv[optind])) == 1) //if it is a file, unlink the file
		{
			unlink((argv[optind]));
			exit(EXIT_SUCCESS);
		}
		else if ((check_type(argv[optind])) == 0) //else if it's a directory, run our function
		{
			unlink_dir_depth(argv[optind], aflag, dflag, depth);
			exit(EXIT_SUCCESS);
		}
	}
	if (dflag == 1 && uflag == 0)
	{
		if (depth < 0)
		{
			fprintf(stderr, "Error: %s \nDepth specified should not be negative.\nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
		}
		list_directory_depth(argv[optind], aflag, lflag, depth);
		exit(EXIT_SUCCESS);
	}
	if (argv[optind + 1] != NULL) 
	{
		//If there are stat expressions options
		STAT_EXPRESSION statexpr = compile_stat_expression(argv[optind + 1]);
		list_directory_stat_exp(argv[optind], aflag, lflag, statexpr);
		exit(EXIT_SUCCESS);
	}
	
	if ((check_type(argv[optind])) == 1) 
	{
		printf("%s\n", argv[optind]);
		exit(EXIT_SUCCESS);
	}
	else if ((check_type(argv[optind])) == 0) 
	{
		list_directory(argv[optind], aflag, lflag);
		exit(EXIT_SUCCESS);
	}
	return 0;
}
