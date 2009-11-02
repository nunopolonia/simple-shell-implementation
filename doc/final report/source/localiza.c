/* cmds.c */

int cmd_localiza(char* cmd) {
  char *init_path = "/home/nunopolonia/work/inqueritos";
  char search_string[MAX_CANON];

  /* Returns search string without "localiza " */
  strcpy(search_string, cmd+9);

  /* makes a depth-first search starting at "init_path" searching for search_string */
  depthsearch(init_path, search_string);

  return 0;
}

/* aux.c */

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

