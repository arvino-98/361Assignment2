#include "sh.h"
#include "builtins.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal);

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */
  if (signal(SIGINT, SIG_IGN) != SIG_IGN){
    signal(SIGINT, sig_handler);
  }

  return sh(argc, argv, envp);
}

void sig_handler(int sig)
{
  /* define your signal handler */
  signal(sig, SIG_IGN);
  printf("\n Cannot be terminated with Ctrl+C \n");
}
