#ifndef __AUX_H__
#define __AUX_H__

#include "history.h"

/* struct created for the stats function to have the character and it's frequence */
typedef struct {
   char c;
   int frequence;
} character;

/* struct created to send commands to threads */
typedef struct {
  int verbose;
  int *frequences;
} arg;


/* 1st phase functions */
void soshreadline(char *clean_line);

void exitfunction();

int makeargv(char *s, char *delimiters, char ***argvp);

void freemakeargv(char **argv);

void depthsearch(char *path, char *search_string);

void print_tree_below(char *path);

/* 2nd phase functions */

int sendtexttoserver(char *cmd);

char *strlwr(char *string);

void print_alphabet(int *alphabet);

int copyfd(int fromfd, int tofd);

void *readandcounttext(void *v);

void *readandrespondtocmds(void *v);

/* comparing function for qsort */
int cmpintp(const void *p1, const void *p2);

#endif /* __AUX_H__ */

