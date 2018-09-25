#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>


int isBuiltIn(char *command, char **args);
int builtInSize();
void getBuiltInPtr(char *command,char **args);

void bic_exit();
void bic_pwd();
void bic_cd(char **args);
void bic_list(char **args);
void bic_history(char **args);
void bic_pid();
