#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

int check_type(char *pathname);

void list_directory(char *dirname, bool aflag, bool lflag, char *options);

void list_directory_stat_exp(char *dirname, bool aflag, bool lflag, char *options);

void list_directory_depth(char *dirname, bool aflag, bool lflag, int depth, char options);

int count_all(char *dirname, bool aflag, int filecount);

void unlink_dir(char *pathname, bool aflag);

void unlink_dir_depth(char *pathname, bool aflag, bool dflag, int depth);

int read_args (int argc, char *argv[]);
