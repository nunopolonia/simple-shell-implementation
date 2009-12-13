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
    if(strncmp(clean_line, "!", 1) != 0) {
      history_add(clean_line);
      /* sends the output to a named pipe so that it can be read by freq */
      sendtexttoserver(clean_line);
    } else {
      /* if it's a search, finds the last command sends it to processing
      ** instead of the line received in the stdin */
      command = history_search(clean_line);

      /* if a command was found that satisfies the search */
      if(command != NULL) {
        history_add(command);
        sendtexttoserver(clean_line);
        printf("command found: %s\n", command);
        /* clears the memory block used to receive every command,
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
	int input;

  /* we are using the write funcion instead of printf because it isn't afected by signals */
  write(STDERR_FILENO, question, question_len);
  while(TRUE) {
    printf("> ");
    /* TODO catch bad input with more than one letter */
    input = fgetc(stdin);
    /* catches the newline character in the end of the input */
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
        /* parent process waiting for children */
        while( ( waitchild = waitpid(-1, NULL, WNOHANG) ) ) {
          if( (waitchild == -1) && (errno != EINTR) )
            break;
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

/* send commands to the sosh.canal named pipe */
int sendtexttoserver(char *cmd) {
  int len;
  char requestbuf[PIPE_BUF];
  int requestfd;

  if ((requestfd = open("/tmp/sosh.canal", O_RDWR)) == -1) {
    perror("Client failed to open log fifo for writing");
    return 1;
  }

  snprintf(requestbuf, PIPE_BUF, "%s", cmd);
  len = strlen(requestbuf);

  if (write(requestfd, requestbuf, len) != len) {
    perror("Client failed to write");
    return 1;
  }

  close(requestfd);
  return 0;
}

/* takes a string and puts every character in lowercase */
char *strlwr(char *string) {
  char *p = string;

  while (p && *p) {
    *p = tolower(*p);
    p++;
  }

  return string;
}

/* takes the frequences array and prints it */
void print_alphabet(int *alphabet) {
  int i;

  /* for each element in the array... */
  for (i = 0; i < 26; i++) {
    /* add the letter a ascii offset to print the character */
    printf("%c - %d | ", (char)(i+97), alphabet[i]);
  }

  printf("\n");

  return;
}

int copyfd(int fromfd, int tofd) {
  char buf[LINE_MAX];
  int bytesread, byteswritten;
  int totalbytes = 0;

  for ( ; ; ) {
    if( (bytesread = read(fromfd, buf, LINE_MAX)) <= 0 )
      break;
    if( (byteswritten = write(tofd, buf, bytesread)) == -1 )
      break;
    totalbytes += byteswritten;
  }
  return totalbytes;
}

void *readandcounttext(void *v) {
  int requestfd, rval, i;
  char textbuffer[LINE_MAX] = {}, *lwrbuffer;

  arg *arguments = (arg*) v;

  int verbose = (int) arguments->verbose;
  int *frequences = (int*) arguments->frequences;

  /* create a named pipe to receive text */
  if ((mkfifo("/tmp/sosh.canal", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return NULL;
  }

  /* open a read/write communication endpoint to the sosh.canal pipe */
  if ((requestfd = open("/tmp/sosh.canal", O_RDWR)) == -1) {
     perror("Server failed to open its FIFO");
     return NULL;
  }

  /* while cycle reading the commands */
  while( TRUE ) {
    rval = read(requestfd, textbuffer, LINE_MAX);
    if(verbose && rval > 0) { printf("Read %d characters from sosh.canal\n", rval); }

    if (rval == -1) {
      fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
      return NULL;
    }

    /* lower every character */
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

  return NULL;
}

void *readandrespondtocmds(void *v) {
  int cmdsfd, resultsfd, bytesread, i;
  char cmdbuffer[LINE_MAX] = {};
  character *alphabet[26];

  arg *arguments = (arg*) v;

  int verbose = (int) arguments->verbose;
  int *frequences = (int*) arguments->frequences;

  /* create a named pipe to handle incoming requests */
  if ((mkfifo("/tmp/sosh.cmd", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return NULL;
  }

  /* create a named pipe to send the incoming requests results */
  if ((mkfifo("/tmp/sosh.results", FIFO_PERMS) == -1) && (errno != EEXIST)) {
     perror("Server failed to create a FIFO");
     return NULL;
  }

  /* open a read communication endpoint to the pipe */
  if ((cmdsfd = open("/tmp/sosh.cmd", O_RDONLY)) == -1) {
     perror("Server failed to open its FIFO");
     return NULL;
  }

  /* open a write communication endpoint to the pipe */
  if ((resultsfd = open("/tmp/sosh.results", O_WRONLY)) == -1) {
     perror("Server failed to open its FIFO");
     return NULL;
  }

  /* while cycle reading the commands */
  while( TRUE ) {

    bytesread = read(cmdsfd, cmdbuffer, LINE_MAX);
    if(verbose && bytesread > 0) { printf("Read %d characters from sosh.cmd\n", bytesread); }

    if (bytesread == -1) {
      fprintf(stderr, "failed to read from pipe: %s\n", strerror(errno));
      return NULL;
    }

    if(strcmp(cmdbuffer, "stats") == 0) {
      for( i = 0; i < 27; i++) {
        alphabet[i]->c = (char) i+97;
        alphabet[i]->frequence = frequences[i];
      }
      for( i = 0; i < 27; i++) {
        printf("%c - %d | ", alphabet[i]->c, alphabet[i]->frequence);
        printf("\n");
      }
    }
  }

  return NULL;
}

/* quicksort implementation taken from http://alienryderflex.com/quicksort/ */
void swap(int *a, int *b)
{
  int t=*a; *a=*b; *b=t;
}
void sort(int arr[], int beg, int end)
{
  if (end > beg + 1)
  {
    int piv = arr[beg], l = beg + 1, r = end;
    while (l < r)
    {
      if (arr[l] <= piv)
        l++;
      else
        swap(&arr[l], &arr[--r]);
    }
    swap(&arr[--l], &arr[beg]);
    sort(arr, beg, l);
    sort(arr, r, end);
  }
}

