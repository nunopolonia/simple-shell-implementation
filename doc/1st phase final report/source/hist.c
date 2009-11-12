/* history.h */

/* declaration of global scoped list of commands entered */
history_t *history_list;

/* history.c */
void using_history() {
   history_list = (history_t*) malloc(sizeof(history_t));
   history_list->first = NULL;
   history_list->last = NULL;
   
   return;
}

void history_destroy(history_t *history) {
   free(history);
   
   return;
}


void history_add(char *string) {
   history_item_t *new = NULL, *previous_last = NULL;
   char *new_string;
   
   /* since we're using a single memory block for every command, we have to create space in
   ** memory for each command in history or else they would all point to the last command */
   new = (history_item_t*) malloc(sizeof(history_item_t));
   new_string = malloc(sizeof(char)*(strlen(string)+1));
   strcpy(new_string, string);
   
   /* initialization of the new item structure */
   new->string = new_string;
   new->next = NULL;
   new->previous = NULL;
   
   /* if the list is not empty */
   if(history_list->last != NULL) {
    previous_last = history_list->last;
    previous_last->next = new;
    new->previous = previous_last;
    history_list->last = new;
   /* if the command is the first inserted */
   } else {
    history_list->first = new;
    history_list->last = new;
   }
  
  return;
}

void history_print() {
  history_item_t *current = history_list->first;
  int count = 1;

  while (current != NULL) {
     printf("%d: %s\n", count, current->string);
     current = current->next;
     count++;
  }
  
  return;
}

/* cmds.c */
int cmd_hist() {
  
  history_print(history_list);
  
  return 0;
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

