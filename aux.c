#include "aux.h"

void exitfunction() {
  char confirmation[] = "Deseja terminar o programa? (s/n)\n";
  int confirmation_len = sizeof(confirmation);
  char answer[max_char];
  
  write(STDERR_FILENO, confirmation, confirmation_len);
  while( fgets(answer, max_char, stdin) ) {
    if( strcmp(answer,"s\n") == 0 ) {
      exit(0);
    }
    else if( strcmp(answer,"n\n") == 0 ) {
      break;
    }
    else
      continue;
  }
  return;
}

char *soshReadline(const char *prompt) {
	static char *line_read;
	
	// If the buffer has already been allocated,
	// return the memory to the free pool.
	if (line_read) {
		free(line_read);
		line_read = (char *)NULL;
	}

	// Get a line from the user.
	line_read = readline(prompt);

	// If the line has any text in it, save it on the history.
	//if (line_read && *line_read)
	//	add_history(line_read);

	return (line_read);
}

