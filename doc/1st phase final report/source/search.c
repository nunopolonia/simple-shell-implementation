/* history.h */

/* declaration of global scoped list of commands entered */
history_t *history_list;

/* history.c */
char *history_search(char *cmd) {
  history_item_t *current = history_list->last;
  char *search_string = NULL;
  
  /* remove the ! from the beginning of the sentence */
  search_string = strtok(cmd, "!");
  
  /* search the history backwards */
  while (current != NULL) {
     if( strstr(current->string, search_string) != NULL )
        return current->string;
     else
      current = current->previous;
  }

  return NULL;
}

/* aux.c */

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
