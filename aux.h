#ifndef __AUX_H__
#define __AUX_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>


#define max_char 8192
#define TRUE 1
#define FALSE 0

char *soshReadline(const char *prompt);

void exitfunction();

int makeargv(const char *s, const char *delimiters, char ***argvp);

void freemakeargv(char **argv);


#endif /* __AUX_H__ */

