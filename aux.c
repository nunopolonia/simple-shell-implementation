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
	static char *line_read, *expansion;
	int result;
	
	/* If the buffer has already been allocated, return the memory to the free pool */
	if (line_read) {
		free(line_read);
		line_read = (char *)NULL;
	}

	/* Get a line from the user */
	line_read = readline(prompt);
  
  /* Try to expand the string if the user's searching for an old command */
  result = history_expand (line_read, &expansion);

  /* If there was no expansions or the user doesn't want to execute it */
  if (result < 0 || result == 2)
    free (expansion);

  /* If there was an expansion print the name of the command who's going to be executed */
  if (result == 1) {
    fprintf (stderr, "%s\n", expansion);
    strncpy (line_read, expansion, sizeof (expansion) );
    free (expansion);
  } 

	/* If the line has any text in it, save it on the history */
	if (line_read && *line_read)
		add_history(line_read);

	return (line_read);
}

/* Taken from Unix Systems Programming, Robbins & Robbins, p37 */
int makeargv(const char *s, const char *delimiters, char ***argvp) {
   int error;
   int i;
   int numtokens;
   const char *snew;
   char *t;

   if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
      errno = EINVAL;
      return -1;
   }
   *argvp = NULL;
   snew = s + strspn(s, delimiters);
   if ((t = malloc(strlen(snew) + 1)) == NULL)
      return -1;
   strcpy(t,snew);
   numtokens = 0;
   if (strtok(t, delimiters) != NULL)
      for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ;

   if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
      error = errno;
      free(t);
      errno = error;
      return -1;
   }

   if (numtokens == 0)
      free(t);
   else {
      strcpy(t,snew);
      **argvp = strtok(t,delimiters);
      for (i=1; i<numtokens; i++)
         *((*argvp) +i) = strtok(NULL,delimiters);
   }
   *((*argvp) + numtokens) = NULL;
   return numtokens;
}

void freemakeargv(char **argv) {
   if (argv == NULL)
      return;
   if (*argv != NULL)
      free(*argv);
   free(argv);
}


