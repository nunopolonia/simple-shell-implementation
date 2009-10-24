/*
 * history.c - implementation of the history functions 
 * based on a integer list given by Prof. Rui Maranhão in
 * http://web.fe.up.pt/~rma/SOPE/tp/tp1/aula1.tgz
 */
#include "history.h"

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
   
   new = (history_item_t*) malloc(sizeof(history_item_t));
   new->string = string;
   new->next = NULL;
   
   if(history_list->last != NULL) {
    previous_last = history_list->last;
    previous_last->next = new;
    history_list->last = NULL;
    history_list->last = new;
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
