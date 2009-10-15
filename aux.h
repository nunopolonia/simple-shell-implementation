#ifndef __AUX_H__
#define __AUX_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include <readline/readline.h>
#include <readline/history.h>


#define max_char 8192
#define TRUE 1
#define FALSE 0

void exitfunction();

char *soshReadline(const char *prompt);


#endif /* __AUX_H__ */

