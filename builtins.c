#include "builtins.h"

const char* BUILT_IN_COMMANDS[] = {
  "pwd",
  "cd"
};
void (*BUILT_IN_COMMANDS_PTR[])(char** args) = {
  bic_pwd,
  bic_cd
};

int builtInSize(){
  return sizeof(BUILT_IN_COMMANDS) / sizeof(char *);
}

// 1 true, else 0
int isBuiltIn(char *command, char **args){
  for (int i = 0; i < builtInSize(); i++){
    if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
      return 1;
    }
  }
  return 0;
}

void getBuiltInPtr(char *command, char **args){
  if (isBuiltIn(command, args) == 1){
    for (int i = 0; i < builtInSize(); i++){
      if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
        (*BUILT_IN_COMMANDS_PTR[i])(args);
      }
    }
  }
}

void bic_exit(){
  exit(3);
}

void bic_pwd(){
  char *tmp;
  tmp = getcwd(NULL, 0);
  printf("%s\n", tmp);
  free(tmp);
}

void bic_cd(char **args){
  chdir(args[1]);
}
