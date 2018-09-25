#include "history.h"

void insert(char *cmd){
  HistList *newHist = malloc(sizeof(HistList));
  newHist->data = malloc(128);
  strcpy(newHist->data,cmd);
  newHist->next = NULL;

  // add it to list
  if (head == NULL){
    head = newHist;
  }
  else{
    HistList *temp = head;
    while (temp->next != NULL){ // move to end
      temp = temp->next;
    }
    temp->next = newHist;
  }
}
