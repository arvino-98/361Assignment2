#include "builtins.h"

const char* BUILT_IN_COMMANDS[] = {
  "exit"
};
void (*BUILT_IN_COMMANDS_PTR[])() = {
  bic_exit
};

int builtInSize(){
  return sizeof(BUILT_IN_COMMANDS) / sizeof(char *);
}

// 1 true, else 0
int isBuiltIn(char *command){
  for (int i = 0; i < builtInSize(); i++){
    if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
      return 1;
    }
  }
  return 0;
}

void getBuiltInPtr(char *command){
  if (isBuiltIn(command) == 1){
    for (int i = 0; i < builtInSize(); i++){
      if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
        (*BUILT_IN_COMMANDS_PTR[i])();
      }
    }
  }
}

void bic_exit(){
  exit(3);
}
