#include "cmds.h"

int main (int argc, char *argv[]) {
  int requestfd, rval, i, j;
  char buffer[LINE_MAX] = {}, *lwrbuffer;
  int *frequences;

  frequences = (int*) calloc (26,sizeof(int));

  /*TODO change the name of the fifo to a global variable */
  /* create a named pipe to handle incoming requests */
  if ((mkfifo("/tmp/sosh.canal", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return 1;
  }
  /* open a read/write communication endpoint to the pipe */
  if ((requestfd = open("/tmp/sosh.canal", O_RDWR)) == -1) {
     perror("Server failed to open its FIFO");
     return 1;
  }

  /* while cycle reading the commands */
  while( TRUE ) {
    rval = read(requestfd, buffer, LINE_MAX);
    printf("Just read smth, rval=%d\n", rval);
    if (rval == -1) {
      fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
      return 1;
    }

    /* lower every character */
    lwrbuffer = strlwr(buffer);

    /* read the string and count the ocurrences of each letter */
    for (i = 0; i < rval; i++) {
      if( (int)lwrbuffer[i] > 96 && (int)lwrbuffer[i] < 123 ) {
        int index = (int)lwrbuffer[i] - 97;
        frequences[index]++;
        printf("%c", lwrbuffer[i]);
        printf("counted smth\n");
      }
    }

    /* print alphabet */
    for (j = 0; j < 26; j++) {
      printf("%c - %d | ", (char)(j+97), frequences[j]);
    }
    printf("\n");
  }

  return 0;
}

