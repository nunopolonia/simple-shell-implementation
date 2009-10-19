#ifndef __AUX_H__
#define __AUX_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

char *soshreadline();

void exitfunction();

int makeargv(const char *s, const char *delimiters, char ***argvp);

void freemakeargv(char **argv);


#endif /* __AUX_H__ */

