#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int isBuiltIn(char *command);
int builtInSize();
void getBuiltInPtr(char *command);

void bic_exit();