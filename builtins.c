#include "builtins.h"
#include "history.h"
int histToPrint = 10;

const char* BUILT_IN_COMMANDS[] = {
  "pwd",
  "cd",
  "list",
  "history",
  "pid"
};
void (*BUILT_IN_COMMANDS_PTR[])(char** args) = {
  bic_pwd,
  bic_cd,
  bic_list,
  bic_history,
  bic_pid
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
  DIR *dir;
  struct dirent *dp;
  if (args[1] != NULL) // if called with argument
  {
    dir = opendir(args[1]); // open and set it as our dir
    if (!dir) // only if argument is a valid directory
    {
      printf("Invalid directory\n");
      return;
    }
  }
  else // else set dir to current working directory
  {
    char *cwd = getcwd(NULL, 0);
    dir = opendir(cwd);
    free(cwd);
  }

  // then print dir
  while ((dp=readdir(dir)) != NULL)
  {
    printf("%s\n", dp->d_name);
  }
  free(dir);
} /* list() */

void bic_history(char **args){
  if (args[1] != NULL)
  {
    int isDigit = 1;
    for (int i = 0; i < strlen(args[1]); i++){
      if (isdigit(args[1][i]) == 0){
        isDigit = 0;
      }
    }
    if (isDigit){
      histToPrint = atoi(args[1]);
    }
  }
  else if(args[1] == NULL)
  {
    int i = 0;
    HistList *temp = head;
    while (temp != NULL && i < histToPrint){
      printf("%s\n", temp->data);
      temp = temp->next;
      i++;
    }
  }
}

void bic_pid(){
  printf("PID: %d\n", getpid());
}
