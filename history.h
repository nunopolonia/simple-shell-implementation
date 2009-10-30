/*
 * history.h - definitions and declarations history.c
 * based on a integer list given by Prof. Rui Maranhão in
 * http://web.fe.up.pt/~rma/SOPE/tp/tp1/aula1.tgz
 */
#include "definitions.h"

/* history_item - each element of the history points to the next element */
typedef struct history_item {
   char *string;
   struct history_item *previous;
   struct history_item *next;
} history_item_t;

/* history_t */
typedef struct {
   history_item_t *first;
   history_item_t *last;
} history_t;

/* declaration of global scoped list of commands entered */
history_t *history_list;

/* using_history - allocates memory for history_t and initializes it */
void using_history();

/* history_destroy - free memory of history_t and all its items */
void history_destroy(history_t *);

/* history_add - insert a new item with value 'string' in history 'history' */
void history_add(char* string);

/* history_print - print the content of history 'history' to standard output */
void history_print();

/* history_search - searches the 'history' backwards to find a command previously
** entered that matches the given string */char *history_search(char *cmd);
