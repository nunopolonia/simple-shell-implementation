#ifndef __AUX_H__
#define __AUX_H__

#include "history.h"
#include "restart.h"

/* 1st phase functions */
void soshreadline(char *clean_line);

void exitfunction();

int makeargv(char *s, char *delimiters, char ***argvp);

void freemakeargv(char **argv);

void depthsearch(char *path, char *search_string);

void print_tree_below(char *path);

/* 2nd phase functions */

int sendtoserver(char *cmd);

void countstringletters(int flag, char string);

#endif /* __AUX_H__ */

