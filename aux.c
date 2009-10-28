#include "aux.h"

/* sosh readline function */
void soshreadline(char *clean_line) {
	char line[LINE_MAX] = "";
	int line_size = 0;
	char *command = NULL;
	
	/* cleans the input buffer */
  memset(clean_line, 0, LINE_MAX);
  
	printf("> ");
	if( fgets(line, LINE_MAX, stdin) != NULL ) {
    line_size = strlen(line);
    strncpy(clean_line, line, line_size-1); 

    /* adds the new command to history except if it's a search */
    if(strncmp(clean_line, "!", 1) != 0)
      history_add(clean_line);
    else {
      /* if it's a search, finds the last command sends it to processing
      ** instead of the line received in the stdin */
      command = history_search(clean_line);
      
      /* if a command was found that satisfies the search */
      if(command != NULL) {
        history_add(command);
        printf("command found: %s\n", command);
        /* clears the memory block used to received every command,
        ** we pass that command for parsing */
        memset(clean_line, 0, LINE_MAX);
        line_size = strlen(command);
        strncpy(clean_line, command, line_size);
      } else
        printf("Command not found in history\n");
    }    
  }

	return;
}

void exitfunction() {
  char question[] = "\nDeseja terminar o programa? (s/n)\n";
  int question_len = sizeof(question);
  char error[] = "Os parametros aceites são (s) ou (n)\n";
  int error_len = sizeof(error);
  int input;
  
  /* we are using the write funcion instead of printf because it isn't afected by signals */
  write(STDERR_FILENO, question, question_len);
  while(TRUE) {
    printf("> ");
    /* TODO catch bad input with more than one letter */
    input = fgetc(stdin);
    /* catches the newline caracter in the end of the input */
    getchar();
    
    if( input == 's' ) {
      /* destroys the space ocuppied by history list from memory */
      printf("Terminating sosh...\n");
      history_destroy(history_list);
      exit(0);
    }
    else if( input == 'n' ) {
      break;
    }
    else
      write(STDERR_FILENO, error, error_len);
      continue;
  }
  
  return;
}

/* Taken from Unix Systems Programming, Robbins & Robbins, p37 */
int makeargv(char *s, char *delimiters, char ***argvp) {
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


