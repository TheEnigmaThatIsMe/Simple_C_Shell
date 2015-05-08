#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>


int getDirectory(void);

int moveDirectories(char* filepath);

int readDirectory(char* directory);

int grepCommand(char* substring, char* filename);

void execute(char **argv);