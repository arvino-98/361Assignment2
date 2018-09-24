#include "builtins.h"

const char* BUILT_IN_COMMANDS[] = {
  "pwd",
  "cd",
  "list"
};
void (*BUILT_IN_COMMANDS_PTR[])(char** args) = {
  bic_pwd,
  bic_cd,
  bic_list
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

void bic_list (char **args)
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
  DIR *dir;
  if (args[1] != NULL)
  {
    dir = opendir(args[1]);
    if (dir)
    {
      struct dirent *dp;
      while ((dp=readdir(dir)) != NULL)
      {
        printf("%s\n", dp->d_name);
      }
    }
  }
  else
  {
    char *cwd = getcwd(NULL, 0);
    dir = opendir(cwd);
    struct dirent *dp;
    while ((dp=readdir(dir)) != NULL)
    {
      printf("%s\n", dp->d_name);
    }
    free(cwd);
  }
  free(dir);
} /* list() */
