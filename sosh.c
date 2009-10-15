#include <errno.h>
#include <sys/wait.h>
#include "aux.h"
#include "cmds.h"


int main (int argc, char *argv[]) {
  char *cmd;
  pid_t childpid;
  sigset_t mask, orig_mask;

  while(TRUE) {      
    /* Signal install for Ctrl+C TODO switch with sigaction */
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
      /* "quem" process call */
      if( strcmp(cmd, "quem") == 0 ) { cmd_quem(); }
      /* "psu" process call */
      else if( strcmp(cmd, "psu") == 0 ) { cmd_psu(); }
      /* "ver" process call */
      else if( strcmp(cmd, "ver") == 0 ) { cmd_ver(); }
      /* "ajuda" process call */
      else if( strcmp(cmd, "ajuda") == 0 ) { cmd_ajuda(); }
      /* "localiza" process call */
      else if( strcmp(cmd, "localiza") == 0 ) { cmd_localiza(); }
      /* "exit" process call */
      else if( strcmp(cmd, "exit") == 0 ) { cmd_exit(); }
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
