#include "cmds.h"

int main (int argc, char *argv[]) {
  int *frequences, verbose = 0, error, thread_num, i;
  pthread_t *tid, cmd_tid;

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
  while((c=getopt(argc, argv, "v::t:"))!= EOF) {
    switch(c) {
      case 'v':
        verbose = 1;
      break;
      case 't':
        thread_num = atoi(optarg);
      break;
      case '?':
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        exit(EXIT_FAILURE);
      break;
    }
  }

  /* create a named pipe to receive text */
  if ((mkfifo("/tmp/sosh.canal", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return 1;
  }

  frequences = (int*) calloc (26,sizeof(int));

  /* structure initialization */
  arguments.verbose = verbose;
  arguments.frequences = frequences;

  if((tid = (pthread_t *)calloc(thread_num, sizeof(pthread_t))) == NULL) {
    perror("Failed to allocate space for thread id's");
    return 1;
  }

  /* create thread that handles the client commands */
  if( (error = pthread_create(&cmd_tid, NULL, readandrespondtocmds, &verbose)) )
    fprintf(stderr, "Failed to create thread: %s\n", strerror(error));

  /* create <number> of threads that handle the character counting */
  for (i = 0; i < thread_num; i++) {
    if( (error = pthread_create(tid+i, NULL, readandcounttext, (void*) &arguments )) ) {
      fprintf(stderr, "Failed to create thread %d: %s\n", i, strerror(error));
      tid[i] = pthread_self();
    }
  }
    /* makes the server wait for the thread */
  for (i = 0; i < thread_num; i++) {
    if(pthread_equal(pthread_self(), tid[i]))
      continue;

    if( (error = pthread_join(tid[i], NULL)) )
      fprintf(stderr, "Failed to join thread: %s\n", strerror(error));
  }

  /* makes the server wait for the thread */
  if( (error = pthread_join(cmd_tid, NULL)) )
    fprintf(stderr, "Failed to join thread: %s\n", strerror(error));

  free(tid);
  return 0;
}

