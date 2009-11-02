#include "aux.h"

/* sosh readline function */
void soshreadline(char *clean_line) {
	char line[MAX_CANON] = "";
	int line_size = 0;
	char *command = NULL;
	
	/* cleans the input buffer */
  memset(clean_line, 0, MAX_CANON);
  
	printf("> ");
	if( fgets(line, MAX_CANON, stdin) != NULL ) {
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
        memset(clean_line, 0, MAX_CANON);
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
	char line[MAX_CANON] = "";
	int line_size = 0;
  
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

void depthsearch(char *path, char *search_string) {
  struct dirent *direntp;
  struct stat statbuf;
  char mycwd[PATH_MAX];
  DIR *dirp;
  pid_t childpid, waitchild;

  if( chdir(path) == -1 )
    perror("Failed to change working directory");
  
  /* path fetching error handling */
  if( getcwd(mycwd, PATH_MAX) == NULL ) {
    perror("Failed to get current working directory");
    return;
  }
  
  /* open the init_path directory */
  if( (dirp = opendir(mycwd) ) == NULL ) {
    perror("Failed to open directory");
    return;
  }
  
  while( ( direntp = readdir(dirp) ) != NULL ) {
    /* File status error handling */
    if(stat(direntp->d_name, &statbuf) == -1 )
      perror("Failed to get file status");

    /* If the string is in any of the folder files */
    if(strstr(direntp->d_name, search_string) != NULL ) {
      /* if its a dir print all the tree below in a new process so 
      ** we don't mess with the current working directory */
      if( (S_ISDIR(statbuf.st_mode) == TRUE) && (S_ISLNK(statbuf.st_mode) == FALSE) ) {
        printf("%s/%s\n", mycwd, direntp->d_name);
        
        childpid = fork();
      
        /* fork error handling */
        if( childpid == -1 ) {
          perror("Failed to fork\n");
          return;
        }
        /* child code */
        if( childpid == 0 ) {      
          print_tree_below(direntp->d_name);
          return;
        }
        continue;
      /* if its a file print it */
      } else
        printf("%s/%s\n", mycwd, direntp->d_name);
        continue;
    /* else if its a directory fork and run this again */
    } else if( (S_ISDIR(statbuf.st_mode) == TRUE) && (S_ISLNK(statbuf.st_mode) == FALSE)
                && (strcmp(direntp->d_name, ".") != 0) && (strcmp(direntp->d_name, "..") != 0) ) {
      /* Launches a child process for each folder */
      childpid = fork();
      
      /* fork error handling */
      if( childpid == -1 ) {
        perror("Failed to fork\n");
        return;
      }
      /* child code */
      if( childpid == 0 ) {      
        depthsearch(direntp->d_name, search_string);
        return;
      }

      /* parent process waiting for children */
      while( ( waitchild = waitpid(-1, NULL, WNOHANG) ) ) {
        if( (waitchild == -1) && (errno != EINTR) )
          break;
      }
    }    
  }
  /* close init_path directory */
  while( (closedir(dirp) == -1) && (errno == EINTR) );
  
  return;
}

void print_tree_below(char *path) {
  struct dirent *direntp;
  struct stat statbuf;
  char mycwd[PATH_MAX];
  DIR *dirp;
  pid_t childpid, waitchild;

  if( chdir(path) == -1 )
    perror("Failed to change working directory");
  
  /* path fetching error handling */
  if( getcwd(mycwd, PATH_MAX) == NULL ) {
    perror("Failed to get current working directory");
    return;
  }
  
  /* open the init_path directory */
  if( ( dirp = opendir(mycwd) ) == NULL ) {
    perror("Failed to open directory");
    return;
  }
  
  while( ( direntp = readdir(dirp) ) != NULL ) {
    /* File status error handling */
    if(stat(direntp->d_name, &statbuf) == -1 )
      perror("Failed to get file status");

    /* Print the filenames */
    if( (strcmp(direntp->d_name, ".") != 0) && (strcmp(direntp->d_name, "..") != 0) )
      printf("%s/%s\n", mycwd, direntp->d_name);
    
    if( (S_ISDIR(statbuf.st_mode) == TRUE) && (S_ISLNK(statbuf.st_mode) == FALSE)
         && (strcmp(direntp->d_name, ".") != 0) && (strcmp(direntp->d_name, "..") != 0) ) {
      /* Launches a child process for each folder */
      childpid = fork();
      
      /* fork error handling */
      if( childpid == -1 ) {
        perror("Failed to fork\n");
        return;
      }
      /* child code */
      if( childpid == 0 ) {      
        print_tree_below(direntp->d_name);
        return;
      }

      /* parent process waiting for children */
      while( ( waitchild = waitpid(-1, NULL, WNOHANG) ) ) {
        if( (waitchild == -1) && (errno != EINTR) )
          break;
      }
    }    
  }
  /* close init_path directory */
  while( (closedir(dirp) == -1) && (errno == EINTR) );
  
  return;
}


