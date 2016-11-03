/*CITS2002 Project 2 2016
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
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include "statexpr.h"

 // Compile this program with:
 // cc -std=c99 -Wall -Werror -pedantic -o cfind cfind.c

#define	OPTLIST		"acd:lrstu"

int check_type(char *pathname) {
	//checks if pathname specified is a file or a directories
	//use dirent DTYPE
	//if file return 1
	//if dir return 2
	struct stat statbuf;
	if (stat(pathname, &statbuf) == 0) {
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
void list_directory_stat_exp(char *dirname, bool aflag, bool lflag, char *options)
{
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
	STAT_EXPRESSION statexpr = compile_stat_expression(options);

	if (dirp == NULL)
	{
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL)
		{
			/* deal with error and exit */
			printf("Error");
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			continue;
		}

		else {
			sprintf(fullpath, "%s/%s", path, dp->d_name);
			if (evaluate_stat_expression(statexpr, dp->d_name, &statbuf) == 1)
			{
				if (aflag == 1) {
					//filecount++;
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if (lflag == 1)
					{

						if (stat(fullpath, &statbuf) == 0)
						{
							printf("%llu\t", statbuf.st_ino);
							//File permissions
							printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
							printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
							printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
							printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
							printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
							printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
							printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
							printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
							printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
							printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
							printf("\t");

							printf("%hu\t", statbuf.st_nlink);
							if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
							{
								printf("%s\t", pwd->pw_name);
							}
							if ((grp = getgrgid(statbuf.st_gid)) != NULL)
							{
								printf("%s\t", grp->gr_name);
							}
							printf("%jd\t", (intmax_t)statbuf.st_size);
							//Get localized date string
							tm = localtime(&statbuf.st_mtime);
							strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

							printf(" %s\t", datestring);
						}


					}

					puts(fullpath);


				}
				else {
					if (dp->d_name[0] == '.') {
						continue;
					}
					else {
						// filecount++;
						sprintf(fullpath, "%s/%s", path, dp->d_name);
						if (lflag == 1)
						{
							//sprintf(fullpath, "%s/%s", path, dp->d_name);
							if (stat(fullpath, &statbuf) != -1)
							{
								printf("%llu\t", statbuf.st_ino);
								printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
								printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
								printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
								printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
								printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
								printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
								printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
								printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
								printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
								printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
								printf("\t");
								printf("%hu\t", statbuf.st_nlink);
								if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
									printf("%s\t", pwd->pw_name);
								if ((grp = getgrgid(statbuf.st_gid)) != NULL)
									printf("%s\t", grp->gr_name);
								printf("%jd\t", (intmax_t)statbuf.st_size);
								//Get localized date string
								tm = localtime(&statbuf.st_mtime);
								strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

								printf(" %s\t", datestring);
							}

						}

						//if(fullpath!=dirname){
						puts(fullpath);
						//}




					}
				}
				if ((check_type(fullpath)) == 2) {
					list_directory_stat_exp(fullpath, aflag, lflag, options);
				}
			}

			free(fullpath);
			//free_stat_expression(statexpr);
		}

	}
	closedir(dirp);
}
void list_directory(char *dirname, bool aflag, bool lflag, char *options)
{
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
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL)
		{
			/* deal with error and exit */
			printf("Error");
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0)
		{
			continue;
		}
		else {
			if (aflag == 1)
			{
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				if (lflag == 1)
				{
					if (stat(fullpath, &statbuf) == 0)
					{
						printf("%llu\t", statbuf.st_ino);
						//File permissions
						printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
						printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
						printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
						printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
						printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
						printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
						printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
						printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
						printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
						printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
						printf("\t");
						printf("%hu\t", statbuf.st_nlink);
						if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
						{
							printf("%s\t", pwd->pw_name);
						}
						if ((grp = getgrgid(statbuf.st_gid)) != NULL)
						{
							printf("%s\t", grp->gr_name);
						}
						printf("%jd\t", (intmax_t)statbuf.st_size);
						//Get localized date string
						tm = localtime(&statbuf.st_mtime);
						strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
						printf(" %s\t", datestring);
					}
				}
				puts(fullpath);
			}
			else {
				if (dp->d_name[0] == '.') {
					continue;
				}
				else
				{
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if (lflag == 1)
					{
						if (stat(fullpath, &statbuf) != -1)
						{
							printf("%llu\t", statbuf.st_ino);
							printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
							printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
							printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
							printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
							printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
							printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
							printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
							printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
							printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
							printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
							printf("\t");
							printf("%hu\t", statbuf.st_nlink);
							if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
								printf("%s\t", pwd->pw_name);
							if ((grp = getgrgid(statbuf.st_gid)) != NULL)
								printf("%s\t", grp->gr_name);
							printf("%jd\t", (intmax_t)statbuf.st_size);
							//Get localized date string
							tm = localtime(&statbuf.st_mtime);
							strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
							printf(" %s\t", datestring);
						}
					}
					puts(fullpath);
				}
			}

			if ((check_type(fullpath)) == 2) {
				list_directory(fullpath, aflag, lflag, options);
			}
			free(fullpath);
		}

	}
	closedir(dirp);
}
void list_directory_depth(char *dirname, bool aflag, bool lflag, int depth, char options)
{
	DIR *dirp;
	struct dirent *dp;
	char *path = dirname;
	size_t pathlen = strlen(dirname);
	dirp = opendir(dirname);


	if (dirp == NULL)
	{
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL && depth >= 0)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL) {
			/* deal with error and exit */
			printf("Error");
		}
		//printf("%s/%s\n",path, ep->d_name);
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0) {
			continue;
		}
		else {
			if (dp->d_name[0] == '.') {
				continue;
			}
			else {
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				//depth--;
				puts(fullpath);
			}

			if ((check_type(fullpath)) == 2 && depth > 0) {
				//depth++;
				list_directory(fullpath, aflag, lflag, &options);
				depth--;
			}
			free(fullpath);
		}
	}
	closedir(dirp);
}

int count_all(char *dirname, bool aflag, int filecount)
{
	DIR *dirp;
	struct dirent *dp;
	char *path = dirname;
	size_t pathlen = strlen(dirname);
	dirp = opendir(dirname);

	if (dirp == NULL)
	{
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL) {
			/* deal with error and exit */
			printf("Error");
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0) {
			continue;
		}
		else {
			if (aflag == 1) {
				filecount++;
				sprintf(fullpath, "%s/%s", path, dp->d_name);
			}
			else {
				if (dp->d_name[0] == '.') {
					continue;
				}
				else {
					filecount++;
					sprintf(fullpath, "%s/%s", path, dp->d_name);
				}
			}
			if ((check_type(fullpath)) == 2) {
				filecount = count_all(fullpath, aflag, filecount);
			}
			free(fullpath);
		}
	}
	closedir(dirp);
	return filecount;
}

void unlink_dir(char *pathname, bool aflag) {
	DIR *dirp;
	struct dirent *dp;
	char *path = pathname;
	size_t pathlen = strlen(pathname);
	dirp = opendir(pathname);
	if (dirp == NULL)
	{
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL)
		{
			/* deal with error and exit */
			printf("Error");
		}
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0) {
			continue;
		}
		else {
			if (aflag == 1) {
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				if ((check_type(fullpath)) == 1) {
					unlink(fullpath);
				}
				else if ((check_type(fullpath)) == 2) {
					//remove the (now empty) directory
					unlink_dir(fullpath, aflag);
					rmdir(fullpath); //remove the (now empty) directory

				}
			}
			else
			{
				if (dp->d_name[0] == '.') {
					continue;
				}
				else {
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					if ((check_type(fullpath)) == 1) {
						unlink(fullpath);
					}
					else if ((check_type(fullpath)) == 2) {
						//remove the (now empty) directory
						unlink_dir(fullpath, aflag);
						if (rmdir(fullpath) == -1) {
							fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
						}
						rmdir(fullpath); //remove the (now empty) directory
					}
				}
			}


		}
		free(fullpath);
	}

	closedir(dirp);
}

void unlink_dir_depth(char *pathname, bool aflag, bool dflag, int depth) {
	DIR *dirp;
	struct dirent *dp;
	char *path = pathname;
	size_t pathlen = strlen(pathname);
	dirp = opendir(pathname);
	if (dirp == NULL)
	{
		exit(EXIT_FAILURE);
	}
	while ((dp = readdir(dirp)) != NULL && depth >= 0)
	{
		char *fullpath = malloc(pathlen + strlen(dp->d_name) + 2);
		if (fullpath == NULL) {
			/* deal with error and exit */
			printf("Error");
		}
		//printf("%s/%s\n",path, ep->d_name);
		if ((strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) == 0) {
			continue;
		}
		else {
			if (aflag == 1) {
				//sprintf(fullpath, "%s/%s", path, dp->d_name);
				sprintf(fullpath, "%s/%s", path, dp->d_name);
				//depth--;
				if (check_type(fullpath) == 1 && depth>0) {
					unlink(fullpath);
				}
				else if (check_type(fullpath) == 1 && depth == 0) {
					unlink(fullpath);
				}
				else if ((check_type(fullpath)) == 2 && depth>0) {
					//depth++;
					rmdir(fullpath); //remove the (now empty) directory
					if (rmdir(fullpath) == -1) {
						fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
					}
					depth--;
					unlink_dir_depth(fullpath, aflag, dflag, depth);
				}
				else if (check_type(fullpath) == 2 && depth == 0) {
					if (rmdir(fullpath) == -1) {
						fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
					}
					rmdir(fullpath); //remove the (now empty) directory
				}
			}
			else {
				if (dp->d_name[0] == '.') {
					continue;
				}
				else {
					sprintf(fullpath, "%s/%s", path, dp->d_name);
					//depth--;
					if (check_type(fullpath) == 1 && depth>0) {
						unlink(fullpath);
					}
					else if (check_type(fullpath) == 1 && depth == 0) {
						unlink(fullpath);
					}
					else if ((check_type(fullpath)) == 2 && depth>0) {
						//depth++;
						rmdir(fullpath); //remove the (now empty) directory
						if (rmdir(fullpath) == -1) {
							fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", fullpath, strerror(errno));
						}
						depth--;
						unlink_dir_depth(fullpath, aflag, dflag, depth);
					}
					else if (check_type(fullpath) == 2 && depth == 0) {
						if (rmdir(fullpath) == -1) {
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

int read_args(int argc, char *argv[]) {
	int  opt;
	bool aflag = false; //all entries be considered, including file-entries beginning with the '.' character
	bool cflag = false; //Print only the count of the number of matching file-entries, then exit
	bool dflag = false;
	bool lflag = false; //print long listing (permissions, inode, number of links, owner's name, group-owner's name, size, modificate-date, and name)
	bool rflag = false; //Reverse the order of any sorting options.
	bool sflag = false; //Print matching file-entries, sorted by size. If both -s and -t are provided, -t takes precedence.
	bool tflag = false; //Print matching file-entries, sorted by modification time. If both -s and -t are provided, -t takes precedence.
	bool uflag = false; //Attempt to unlink (remove) as many matching file-entries as possible. The cfind utility should exit with failure if any attempt to unlink a file-entry was unsuccessful.
	//char *filenm = NULL;
	int  depth = 0; //Limit the search to the indicated depth, descending at most depth levels
	int filecount = 0;
	char *options;


	while ((opt = getopt(argc, argv, OPTLIST)) != -1) {
		//  ACCEPT A BOOLEAN ARGUMENT
		if (opt == 'a')
		{
			aflag = !aflag;
			//read_all(argv[1]);
		}
		else if (opt == 'c')
		{
			cflag = !cflag;
			//countfiles(argv[optind]);
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

	if (argc <= 0) {    //  display program's usage/help
		fprintf(stderr, "Error: %s \nusage: ./cfind [options]  pathname  [stat-expression]\n", strerror(errno));
	}
	if (access(argv[optind], F_OK) != 0)
	{
		fprintf(stderr, "Error: %s: %s \nusage: ./cfind  [options]  pathname  [stat-expression]\n", argv[optind], strerror(errno));
		exit(EXIT_FAILURE); 		//exit indicating failure
	}
	if (argv[optind + 1] != NULL)
	{
		options = argv[optind + 1];
	}
	if (cflag == 1) {
		printf("%d\n", count_all(argv[optind], aflag, filecount));
		exit(EXIT_SUCCESS);
	}
	//copy this
	if (uflag == 1 && dflag == 0) {
		if ((check_type(argv[optind])) == 1) //if it is a file, unlink the file
		{
			unlink((argv[optind]));
			exit(EXIT_SUCCESS);
		}
		else if ((check_type(argv[optind])) == 2) //else if it's a directory, run our function
		{
			unlink_dir(argv[optind], aflag);
			exit(EXIT_SUCCESS);
		}
	}
	if (uflag == 1 && dflag == 1) {
		if ((check_type(argv[optind])) == 1) //if it is a file, unlink the file
		{
			unlink((argv[optind]));
			exit(EXIT_SUCCESS);
		}
		else if ((check_type(argv[optind])) == 2) //else if it's a directory, run our function
		{
			unlink_dir_depth(argv[optind], aflag, dflag, depth);
			exit(EXIT_SUCCESS);
		}
	}
	if (dflag == 1 && uflag == 0) {
		if (depth<0)
		{
			fprintf(stderr, "Error: %s \nDepth specified should not be negative.\nusage: ./cfind  [options]  pathname  [stat-expression]\n", strerror(errno));
		}
		list_directory_depth(argv[optind], aflag, lflag, depth, *options);
		exit(EXIT_SUCCESS);
	}
	if ((check_type(argv[optind])) == 1) //if it is a file, unlink the file
	{
		printf("%s\n", argv[optind]);
		exit(EXIT_SUCCESS);
	}
	else if ((check_type(argv[optind])) == 2) //else if it's a directory, run our function
	{
		//printf("%s\n",argv[optind]);
		list_directory(argv[optind], aflag, lflag, options);
		exit(EXIT_SUCCESS);
	}

	return 0;
}

