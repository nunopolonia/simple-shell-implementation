#include "cmds.h"

int cmd_quem() {
  /* who system call */
  system("who");

  return 0;
}

int cmd_psu() {
  /* lists the user processes */
  system("ps ux");
  
  return 0;
}

int cmd_usrbin(char** cmd) {
  char **myargv;
  char delim[] = {" \t"};
 
  /* If the buffer has already been allocated, return the memory to the free pool */
  freemakeargv(myargv);
 
  /* If makeargv is sucessful parsing the cmd string execute the program */
  if (makeargv(cmd, delim, &myargv) == -1)
    perror("Parent failed to create the argument array\n");
  else { 
  /* execvp completly substitutes the process in memory so the message is showed
  ** if the execvp command fails meaning that we are still in the sosh process */  
    execvp(myargv[0], &myargv[0]);
    printf("Comando não suportado\n");
  }
  return 0;
}

int cmd_exit() {
  /* kills the parent process TODO switch to SIGUSR1 */
  if (kill(getppid(), SIGTERM) == -1)
    perror ("Failed to kill parent");
  /* guarantees that there's no zombie processes */
  if (raise(SIGTERM) != 0)
    perror("Failed to kill self"); 
  
  return 0;
}

int cmd_localiza(char** cmd) {

  /* Open the current directory */
    
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
  HIST_ENTRY **history;
  int i;

  history = history_list ();
  if (history)
    for (i = 0; history_length; i++)
      printf ("%d: %s\n", i + history_base, history[i]->line);
  
  return 0;
}
