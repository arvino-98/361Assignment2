#include "builtins.h"
#include "history.h"
#include "alias.h"
#include "sh.h"
#include <signal.h>
int histToPrint = 10;
char* prevDirectory;
char **bic_envp;

// array of char* that keep track of names of our buil-in commands
// mapped 1 to 1 to function pointers in below array BUILT_IN_COMMANDS_PTR
const char* BUILT_IN_COMMANDS[] = {
  "pwd",
  "cd",
  "history",
  "pid",
  "kill",
  "printenv",
  "setenv",
  "alias"
};
// array of char* that keep track of function pointers of our buil-in commands
// mapped 1 to 1 to function pointers in above array BUILT_IN_COMMANDS
void (*BUILT_IN_COMMANDS_PTR[])(char** args) = {
  bic_pwd,
  bic_cd,
  bic_history,
  bic_pid,
  bic_kill,
  bic_printenv,
  bic_setenv,
  bic_alias
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

// iterates through the BUILT_IN_COMMANDS_PTR array a returns the proper function pointer
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
  char *prevDirectoryLoc = prevDirectory; // local copy of prevDirectory
  prevDirectory = getcwd(NULL, 0); // change global prevDirectory
  if (args[1] == NULL){ // if called with no arguments
    char *home = getenv("HOME");
    if (chdir(home) != 0){
      perror("Error moving to home directory");
    }
  }
  else if (args[1] != NULL && strcmp(args[1], "-") == 0){ // if called with 1 argument: "-"
    if (chdir(prevDirectoryLoc) != 0){
      perror("Not a valid directory");
    }
  }
  else if (args[1] != NULL){ // for any other argument given
    if (chdir(args[1]) != 0){
      perror("Not a valid directory");
    }
  }
  free(prevDirectoryLoc);
}

// prints history or changes amount to print based on given argument
void bic_history(char **args){
  if(args[1] == NULL){ // if called with no arguments, print history
    int i = 0;
    HistList *temp = head;
    while (temp != NULL && i < histToPrint){
      printf("%s\n", temp->data);
      temp = temp->next;
      i++;
    }
  }
  else if (args[1] != NULL){ // if called with 1 argument, change number to print
    int isDigit = 1;
    // if any character in given string is a digit set isDigit to false
    for (int i = 0; i < strlen(args[1]); i++){
      if (isdigit(args[1][i]) == 0){
        isDigit = 0;
      }
    }
    if (isDigit){
      histToPrint = atoi(args[1]);
    }
  }
}

void bic_pid(){
  printf("PID: %d\n", getpid());
}

// kills the procss of a give pid
void bic_kill(char **args){
  if (args[1] != NULL) {
    if (kill(atoi(args[1]), SIGTERM) != 0){
      perror("Not a valid signal");
    }
  }
}

// prints enviroment or single variable depending on argument
void bic_printenv(char **args){
  if(args[1] == NULL){ // if called with no arguments
    char **envpLoc = bic_envp; // local variable to hold environment list
    while(*envpLoc){ // move thru environment list and print each variable
      char *thisEnv = *envpLoc;
      printf("%s\n", thisEnv);
      envpLoc++;
    }
  }
  else if(args[1] != NULL){ //if called with 1 argument
    char *tmp;
    if ((tmp=getenv(args[1])) != NULL){ // set temp to the variable we want and print
      printf("%s\n", tmp);
    }
  }
  else if (args[2] != NULL){ // if called with >1 arguments
    printf("Too many arguments\n");
    return;
  }
}

// sets environment variables according to arguments
// if no argument given, prints entire environment
void bic_setenv(char **args){
  if (args[1] == NULL){ // if called with no arguments
    bic_printenv(args);
  }
  else if(args[3] != NULL){ // if called with >3 arguments
    printf("Too many arguments\n");
    return;
  }
  else if(args[2] != NULL){ // if called with 2 arguments
    if (setenv(args[1], args[2], 1) != 0){
      perror("Error setting variable and value");
    }
  }
  else if(args[1] != NULL){ // if called with 1 argument
    if (setenv(args[1], "", 0) != 0){
      perror("Error setting variable");
    }
  }
}

void bic_alias(char **args){
  if(args[1] == NULL){ // if called with no arguments, print history
    AliasList *temp = aliasHead;
    while (temp != NULL){
      printf("command: %s arguments: ", temp->alias);
      int j = 0;
      while (temp->aliasArgs[j] != NULL){
        printf("%s ", temp->aliasArgs[j]);
        j++;
      }
      printf("\n");
      temp = temp->next;
    }
  }
  else{
    insertAlias(args);
  }
}
