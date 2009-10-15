#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "aux.h"
#include "cmds.h"


int main (int argc, char *argv[]) {
  char *cmd;
  pid_t childpid;
  sigset_t mask, orig_mask;

  while(TRUE) {      
    /* Signal install for Ctrl+C */
    signal(SIGINT, exitfunction);

    cmd = soshReadline("> ");
    
    childpid = fork();

    /* blocking signals before executing children */
    if( (sigfillset(&mask) == -1)  || sigprocmask(SIG_SETMASK, &mask, &orig_mask) == -1 ) {
      perror("Failed to block the signal\n");
      return 1;
    }
        
    /* fork error handling */
    if( childpid == -1 ) {
      perror("Failed to fork\n");
      return 1;
    }
    /* child code */
    if( childpid == 0 ) {
      if( strcmp(cmd, "quem") == 0 )
          cmd_quem();
      else
          printf("Comando não suportado\n");
      
      return 1;
    }
    
    /* parent process restoring signals */
    if(sigprocmask(SIG_SETMASK, &orig_mask, NULL) == -1) {
      perror("Parent failed to restore signal mask\n");
      return 1;
    }
    
    /* parent process waiting for children */
    if( childpid != wait(NULL) ) {
      perror("Parent failed to wait due to signal or error\n");
      return 1;
    }

  } /* while closing bracket */
  
  return 0;
}
