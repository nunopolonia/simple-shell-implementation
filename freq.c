#include "cmds.h"

int main (int argc, char *argv[]) {
  int *frequences, verbose = 0, error;
  pthread_t txt_tid, cmd_tid;

  /* struct created to send variables to the thread */
  typedef struct {
    int verbose;
    int *frequences;
  } arg;

  arg arguments;

  /* option extraction variables */
  int c;
  extern char *optarg;
  extern int optind, opterr, optopt;

  /* option extraction */
  while((c=getopt(argc, argv, "v::"))!= EOF) {
    switch(c) {
      case 'v':
        verbose = 1;
      break;
      case '?':
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        exit(EXIT_FAILURE);
      break;
    }
  }

  frequences = (int*) calloc (26,sizeof(int));

  arguments.verbose = verbose;
  arguments.frequences = frequences;

  /* create thread that handles the client commands */
  if( (error = pthread_create(&cmd_tid, NULL, readandrespondtocmds, &verbose)) )
    fprintf(stderr, "Failed to create thread: %s\n", strerror(error));

  /* create thread that handles the character counting */
  if( (error = pthread_create(&txt_tid, NULL, readandcounttext, (void*) &arguments )) )
    fprintf(stderr, "Failed to create thread: %s\n", strerror(error));

  /* makes the server wait for the thread */
  if( (error = pthread_join(cmd_tid, NULL)) )
    fprintf(stderr, "Failed to join thread: %s\n", strerror(error));

  /* makes the server wait for the thread */
  if( (error = pthread_join(txt_tid, NULL)) )
    fprintf(stderr, "Failed to join thread: %s\n", strerror(error));

  return 0;
}

