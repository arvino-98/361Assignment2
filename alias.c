#include "alias.h"

void insertAlias(char **args){
  AliasList *newAlias = malloc(sizeof(AliasList));

  newAlias->alias = malloc(48);
  strcpy(newAlias->alias,args[1]);

  newAlias->aliasArgs = calloc(48, sizeof(char*));
  int i = 0;
  while (args[i + 2] != NULL){
    newAlias->aliasArgs[i] = malloc(64);
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

void printAllAlias(){
  AliasList *temp = aliasHead;
  while (temp != NULL){
    printf("alias name: %s command: ", temp->alias);
    int j = 0;
    while (temp->aliasArgs[j] != NULL){
      printf("%d-[%s], ", j,temp->aliasArgs[j]);
      j++;
    }
    printf("\n");
    temp = temp->next;
  }
}

void freeAliasList(AliasList *head){
  AliasList *tmp;
  while (head != NULL){
    tmp = head;
    head = head->next;
    free(tmp->alias);
    int i = 0;
    while (tmp->aliasArgs[i] != NULL){
      free(tmp->aliasArgs[i]);
      i++;
    }
    free(tmp->aliasArgs);
    free(tmp);
  }
}
