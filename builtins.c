#include "builtins.h"

const char* BUILT_IN_COMMANDS[] = {
  "exit"
};

int isBuiltIn(char *command){
  for (int i = 0; i < builtInSize(); i++){
    if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
      return 1;
    }
  }
  return 0;
}

int builtInSize(){
  return sizeof(BUILT_IN_COMMANDS) / sizeof(char *);
}
