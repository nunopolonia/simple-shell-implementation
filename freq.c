#include "cmds.h"

int main (int argc, char *argv[]) {
  int requestfd, rval, i = 0;
  char buffer[LINE_MAX];
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
  while(TRUE) {
    rval = r_read(requestfd, buffer, LINE_MAX);
    if (rval == -1) {
      fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
      return 1;
    }
    
    /* read the string and count the ocurrences of each letter */
    while(buffer[i] != '\0') {
      if( (int)buffer[i] > 96 && (int)buffer[i] < 123) {
        int index = (int)buffer[i] - 97;
        frequences[index]++;
      }
      i++;
    }
    
    /* print alphabet */
    for (i = 0; i < 26; i ++) {
      printf("%c - %d\n", (char)(i+97), frequences[i]);
    }
  }

  return 0;
}
