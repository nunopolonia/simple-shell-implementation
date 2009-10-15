#include "aux.h"

void exitfunction() {
  char question[] = "Deseja terminar o programa? (s/n)\n";
  int question_len = sizeof(question);
  char error[] = "Os parametros aceites são (s) ou (n)\n";
  int error_len = sizeof(error);
  char *answer;
  
  write(STDERR_FILENO, question, question_len);
  while(TRUE) {
    answer = soshReadline("> ");
    if( strcmp(answer,"s") == 0 ) {
      exit(0);
    }
    else if( strcmp(answer,"n") == 0 ) {
      break;
    }
    else
      write(STDERR_FILENO, error, error_len);
      continue;
  }
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

