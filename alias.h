#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AliasList{
  char *alias;
  char **aliasArgs;
  struct AliasList *next;
} AliasList;
AliasList *aliasHead;

void insertAlias(char **args);
