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
    printf("sosh: Command not found\n");
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
  fprintf(stdout,"\t!<string> - pesquisa no histórico de comandos pelo último comando que contenha <string>\n");
  fprintf(stdout,"\tstats - mostra a lista de letras que passaram pela consola e sua respectiva frequência\n");
  fprintf(stdout,"\texit - sai do sosh\n");

  return 0;
}

int cmd_hist() {

  history_print(history_list);

  return 0;
}


int cmd_localiza(char* cmd) {
  char *init_path = "/home/nunopolonia/work/inqueritos";
  char search_string[MAX_CANON];

  /* Returns search string without "localiza " */
  strcpy(search_string, cmd+9);

  /* makes a depth-first search starting at "init_path" searching for search_string */
  depthsearch(init_path, search_string);

  return 0;
}

int cmd_stats(int statfd) {
  int len, resultsfd, rval;
  char requestbuf[PIPE_BUF], resultbuf[PIPE_BUF];

  /* open a read only communication endpoint to the pipe */
  if ((resultsfd = open("/tmp/sosh.results", O_RDWR)) == -1) {
     perror("Client failed to open log fifo for writing");
     return 1;
  }

  snprintf(requestbuf, PIPE_BUF, "stats");
  len = strlen(requestbuf);

  if (write(statfd, requestbuf, len) != len) {
    perror("Client failed to write");
    return 1;
  }


  /* Read the result from the sosh.results */
  rval = read(resultsfd, resultbuf, LINE_MAX);
  if (rval == -1) {
    fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
    return 1;
  }

  printf("%*s\n", rval, resultbuf);

  return 0;
}

