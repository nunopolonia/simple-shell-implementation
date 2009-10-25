#include "cmds.h"

int cmd_quem() {
  /* who system call */
  execl("/usr/bin/who", "who", NULL);
  perror("Child failed to exec who"); 
  
  return 0;
}

int cmd_psu() {
  /* lists the user processes */
  execl("/bin/ps", "ps", "ux", NULL);
  perror("Child failed to exec ps"); 
  
  return 0;
}

int cmd_usrbin(char* cmd) {
  char **myargv;
  char delim[] = {" \t\n"};

  /* If makeargv is sucessful parsing the cmd string execute the program */
  if ( makeargv(cmd, delim, &myargv) == -1) {
    perror("Parent failed to create the argument array\n");
  } else {
  /* execvp completly substitutes the process in memory so the message is showed
  ** if the execvp command fails meaning that we are still in the sosh process */  
    execvp(myargv[0], myargv);  
    printf("Command not found\n");
  }
  
  return 0;
}

int cmd_exit() {
  
  history_destroy(history_list);
     
  /* kills the parent process TODO switch to SIGUSR1 */
  if (kill(getppid(), SIGTERM) == -1)
    perror ("Failed to kill parent");
  /* guarantees that there's no zombie processes */
  if (raise(SIGTERM) != 0)
    perror("Failed to kill self"); 
  
  return 0;
}

int cmd_localiza(char* cmd) {

  /* Open the root directory */
  
  /* Cycle through every file */
    
  /* If the file is a folder fork the process and repeat it */
  
  return 0;
}

int cmd_ver() {
  fprintf(stdout,"sosh versão 0.3b\n");
  
  return 0;
}

int cmd_ajuda() {
  fprintf(stdout,"sosh versão 0.3b\n");
  fprintf(stdout,"Comandos suportados:\n");
  fprintf(stdout,"\tquem - mostra os utilizadores autenticados no sistema\n");
  fprintf(stdout,"\tpsu - lista os processos do utilizador actual\n");
  fprintf(stdout,"\tlocaliza <cmd> - localiza os caminhos absolutos em que cmd aparece\n");
  fprintf(stdout,"\tver - indica a versão do sosh\n");
  fprintf(stdout,"\tajuda - disponibiliza esta ajuda\n");
  fprintf(stdout,"\thist - mostra a lista de comandos utilizados pelo utilizador\n");
  fprintf(stdout,"\texit - sai do sosh\n");

  return 0;
}

int cmd_hist() {
  
  history_print(history_list);
  
  return 0;
}
