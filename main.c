#include "sh.h"
#include "builtins.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int sig);

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGTERM, SIG_IGN);

  return sh(argc, argv, envp);
}

void sig_handler(int sig)
{
  /* define your signal handler */
  if (sig == SIGINT){
    signal(SIGINT, sig_handler);
  }
}
