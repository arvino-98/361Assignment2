#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int isBuiltIn(char *command, char **args);
int builtInSize();
void getBuiltInPtr(char *command,char **args);

void bic_exit();
void bic_pwd();
void bic_cd(char **args);
