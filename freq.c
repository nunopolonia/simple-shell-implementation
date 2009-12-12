#include "cmds.h"

int main (int argc, char *argv[]) {
  int requestfd, rval, rval2, i, cmdsfd, resultsfd;
  char textbuffer[LINE_MAX] = {}, *lwrbuffer, cmdbuffer[LINE_MAX] = {};
  int *frequences, verbose = 0;

  /* option extraction */
  int c;
  extern char *optarg;
  extern int optind, opterr, optopt;

  while((c=getopt(argc, argv, "v::"))!= EOF) {
    switch(c) {
      case 'v':
        verbose = 1;
      break;

      case '?':
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        exit(EXIT_FAILURE);
      break;

      default:
        exit(EXIT_SUCCESS);
      break;
    }
  }

  frequences = (int*) calloc (26,sizeof(int));

  /* create a named pipe to receive text */
  if ((mkfifo("/tmp/sosh.canal", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return 1;
  }

  /* create a named pipe to handle incoming requests */
  if ((mkfifo("/tmp/sosh.cmd", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return 1;
  }

  /* create a named pipe to send the incoming requests results */
  if ((mkfifo("/tmp/sosh.results", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return 1;
  }

  /* TODO put the read and count lines into separate function */
  /* TODO put the write results into separate function */

  /* open a read/write communication endpoint to the sosh.canal pipe */
  if ((requestfd = open("/tmp/sosh.canal", O_RDWR)) == -1) {
     perror("Server failed to open its FIFO");
     return 1;
  }

  /* open a read communication endpoint to the pipe */
  if ((cmdsfd = open("/tmp/sosh.cmd", O_RDONLY)) == -1) {
     perror("Server failed to open its FIFO");
     return 1;
  }

  /* open a write communication endpoint to the pipe */
  if ((resultsfd = open("/tmp/sosh.results", O_WRONLY)) == -1) {
     perror("Server failed to open its FIFO");
     return 1;
  }

  /* while cycle reading the commands */
  while( TRUE ) {
    rval = read(requestfd, textbuffer, LINE_MAX);
    rval2 = read(cmdsfd, cmdbuffer, LINE_MAX);
    if(verbose && rval > 0) { printf("Read %d caracters from sosh.canal\n", rval); }
    if(verbose && rval2 > 0) { printf("Read %d caracters from sosh.cmd\n", rval2); }

    if (rval == -1) {
      fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
      return 1;
    }
    if (rval2 == -1) {
      fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
      return 1;
    }

    /* lower every caracter */
    lwrbuffer = strlwr(textbuffer);

    /* read the string and count the ocurrences of each letter */
    for (i = 0; i < rval; i++) {
      if( (int)lwrbuffer[i] > 96 && (int)lwrbuffer[i] < 123 ) {
        int index = (int)lwrbuffer[i] - 97;
        frequences[index]++;
      }
    }
    /* print alphabet */
    if(verbose) { print_alphabet(frequences); }
  }

  if(strcmp(cmdbuffer, "stats") == 0) {
      printf("YEAYYYY\n");
  }

  return 0;
}

