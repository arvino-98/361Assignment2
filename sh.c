#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#include "builtins.h"
#include "history.h"
#define BUFFERSIZE 1024

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start out with*/

  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = '>'; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();



  while ( go )
  {
    /* print your prompt */
    char *cwd = getcwd(NULL, 0);
    printf("%s%s", cwd ,prompt);
    free(cwd);

    /* get command line and process */
    fgets(commandline, 1024, stdin);
    commandline[strlen(commandline) - 1] = '\0';
    char* token = strtok(commandline," ");
    args[0] = token;
    command = args[0];
    int i = 1;
    while (token != NULL){
      token = strtok(NULL," ");
      args[i] = token;
      i++;
    }

    if (command != NULL)
    {
      insert(command);
    /* check for each built in command and implement */
      if (strcmp(command, "exit") == 0){ // if exit, free all allocated space
        struct pathelement *tmp = pathlist;
        struct pathelement *prev = NULL;
        while (tmp != NULL){
          prev = tmp;
          tmp = tmp->next;
          free(prev);
        }
        free(pwd);
        free(commandline);
        free(prompt);
        free(args);
        free(owd);
        free(commandpath);
        freeList(head);
        exit(0);
      }
      else if (strcmp(command, "where") == 0){ // if where, call with argument
        where(args[1], pathlist);
      }
      else if (isBuiltIn(command, args)) // check if one of the other built-ins
      {
        getBuiltInPtr(command, args);
      }

       /*  else  program to exec */
      else
      {
        /* find it */
        // if a command starts with ./ or / check if its an absolute pathlist
        // that is executable
        if ((command[0] == '.' && command[1] == '/') || (command[0] == '/'))
        {
          if (access(command, X_OK) == 0)
          {
            commandpath = command;
          }
        }
        // else if it is not, we check if it is a command somewhere that we find
        // with which()
        else if (command[0] != '.' && command[0] != '/')
        {
          commandpath = which(command, pathlist);
        }
        /* do fork(), execve() and waitpid() */
        if (commandpath != NULL){
          printf("Executing %s\n", command);
          if ((pid = fork()) < 0) {
            perror("Fork error");
  		    }
          else if (pid == 0) {/* child */
            execv(commandpath, args);
            perror("Couldn't execute");
            exit(127);
          }
          /* parent */
  		    if ((pid = waitpid(pid, &status, 0)) < 0){
            perror("Wait error");
          }
        }
        else {
          fprintf(stderr, "%s: Command not found.\n", args[0]);
        }
      }
    }
    else
    {
      continue;
    }
  }


  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   char *buffer = calloc(1, BUFFERSIZE);
   buffer[BUFFERSIZE - 1] = 0;
   while (pathlist) {
     sprintf(buffer,"%s/%s", pathlist->element, command);
     if (access(buffer, F_OK) == 0){
       return buffer;
     }
     pathlist = pathlist->next;
   }
   free(buffer);
   return NULL;
} /* which() */

void where(char *command, struct pathelement *pathlist)
{
  /* similarly loop through finding all locations of command */
  char cmd[64];
  while (pathlist) {
    sprintf(cmd, "%s/%s", pathlist->element, command);
    if (access(cmd, F_OK) == 0){
      printf("%s/%s\n", pathlist->element, command);
    }
    pathlist = pathlist->next;
  }
  return;
} /* where() */
