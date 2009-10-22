#include "aux.h"
#include "cmds.h"


int main (int argc, char *argv[]) {
  char cmd[LINE_MAX] = "";
  pid_t childpid;
  sigset_t mask, orig_mask;
  struct sigaction act; 

  /* Signal install for Ctrl+C TODO switch with sigaction */
  act.sa_handler = exitfunction; 
  act.sa_flags = 0; 
  if ( (sigemptyset(&act.sa_mask) == -1) || (sigaction(SIGINT, &act, NULL) == -1) ) 
   perror("Failed to set SIGINT to handle Ctrl-C"); 

  /* SOSH MAIN LOOP */
  while(TRUE) {
    /* input function */
    soshreadline(cmd);


    /* blocking signals before executing children */
    if( (sigfillset(&mask) == -1)  || sigprocmask(SIG_SETMASK, &mask, &orig_mask) == -1 ) {
      perror("Failed to block the signal\n");
      return 1;
    }    

    /* Launches a child process for each command */
    childpid = fork();
        
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
      else if( strcmp(cmd, "localiza") == 0 ) {
        char **myargv;
        char delim[] = {" \t"};
        
        /* If the buffer has already been allocated, return the memory to the free pool */
        freemakeargv(myargv);

        if (makeargv(cmd, delim, &myargv) == -1)
          perror("Parent failed to create the argument array\n");
        else
          cmd_localiza(myargv);
      }
      /* "exit" process call */
      else if( strcmp(cmd, "exit") == 0 ) { cmd_exit(); }
      /* "hist" process call */
      else if( strcmp(cmd, "hist") == 0 ) { cmd_hist(); }
      /* tries to run processes in /usr/bin folders */ 
      else { 
        cmd_usrbin(cmd);
      }
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
