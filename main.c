#include "sh.h"
#include "builtins.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal);

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */

  printf("%d\n", builtInSize());
  printf("%d\n", isBuiltIn("exit"));
  return sh(argc, argv, envp);
}

void sig_handler(int signal)
{
  /* define your signal handler */
}
