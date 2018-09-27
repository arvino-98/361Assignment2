#include "builtins.h"
#include "history.h"
#include <signal.h>
int histToPrint = 10;
char* prevDirectory;
char **bic_envp;

const char* BUILT_IN_COMMANDS[] = {
  "pwd",
  "cd",
  "list",
  "history",
  "pid",
  "kill",
  "printenv"
};
void (*BUILT_IN_COMMANDS_PTR[])(char** args) = {
  bic_pwd,
  bic_cd,
  bic_list,
  bic_history,
  bic_pid,
  bic_kill,
  bic_printenv
};

void initPrevDirectory(){
  prevDirectory = getcwd(NULL, 0);
}
void freePrevDirectory(){
  free(prevDirectory);
}
void initEnvp(char **envp){
  bic_envp = envp;
}

// returns the size of our BUILT_IN_COMMANDS array
int builtInSize(){
  return sizeof(BUILT_IN_COMMANDS) / sizeof(char *);
}

// returns 1 if command is a built-in, else 0
int isBuiltIn(char *command, char **args){
  for (int i = 0; i < builtInSize(); i++){
    if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
      return 1;
    }
  }
  return 0;
}

// iterates through the BUILT_IN_COMMANDS_PTR array a returns the proper function
// according to the given command
void getBuiltInPtr(char *command, char **args){
  for (int i = 0; i < builtInSize(); i++){
    if (strcmp(command, BUILT_IN_COMMANDS[i]) == 0){
      (*BUILT_IN_COMMANDS_PTR[i])(args);
    }
  }
}

// prints the current working directory
void bic_pwd(){
  char *tmp;
  tmp = getcwd(NULL, 0);
  printf("%s\n", tmp);
  free(tmp);
}

// changes directory according to given argument
void bic_cd(char **args){
  char *prevDirectoryLoc = prevDirectory;
  prevDirectory = getcwd(NULL, 0);
  if (strcmp(args[1], "-") == 0){
    if (chdir(prevDirectoryLoc) != 0){
      perror("Not a valid directory");
    }
  }
  else if (args[1] != NULL){
    if (chdir(args[1]) != 0){
      perror("Not a valid directory");
    }
  }
  free(prevDirectoryLoc);
}

// lists all files in current directory
void bic_list (char **args)
{
  DIR *dir;
  struct dirent *dp;
  if (args[1] != NULL){ // if called with argument
    dir = opendir(args[1]); // open and set it as our dir
    if (!dir){ // only if argument is a valid directory
      printf("Invalid directory\n");
      return;
    }
  }
  else{// else set dir to current working directory
    char *cwd = getcwd(NULL, 0);
    dir = opendir(cwd);
    free(cwd);
  }

  // then print dir
  while ((dp=readdir(dir)) != NULL){
    printf("%s\n", dp->d_name);
  }
  free(dir);
} /* list() */

void bic_history(char **args){
  if (args[1] != NULL){
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
  else if(args[1] == NULL){
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

void bic_kill(char **args){
  if (args[1] != NULL) {
    if (kill(atoi(args[1]), SIGTERM) == -1){
      perror("Not a valid signal");
    }
  }
}

void bic_printenv(char **args){
  if(args[1] == NULL){
    char **envpLoc = bic_envp;
    while(*envpLoc){
      char *thisEnv = *envpLoc;
      printf("%s\n", thisEnv);
      envpLoc++;
    }
  }
  else if (args[2] != NULL){
    printf("Too many arguments\n");
  }
  else if(args[1] != NULL){
    char *tmp;
    if ((tmp=getenv(args[1])) != NULL){
      printf("%s\n", tmp);
    }
  }
}
