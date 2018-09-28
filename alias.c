#include "alias.h"

void insertAlias(char **args){
  AliasList *newAlias = malloc(sizeof(AliasList));

  newAlias->alias = malloc(48);
  strcpy(newAlias->alias,args[1]);

  newAlias->aliasArgs = calloc(48, sizeof(char*));
  int i = 0;
  while (args[i + 2] != NULL){
    newAlias->aliasArgs[i] = calloc(48, sizeof(char*));
    strcpy(newAlias->aliasArgs[i], args[i + 2]);
    i++;
  }

  newAlias->next = NULL;

  // add it to list
  if (aliasHead == NULL){
    aliasHead = newAlias;
  }
  else{
    AliasList *temp = aliasHead;
    while (temp->next != NULL){ // move to end
      temp = temp->next;
    }
    temp->next = newAlias;
  }
}
