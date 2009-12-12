#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

/*
** System Includes
*/
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>


/*
** Definitions
*/
#define TRUE 1
#define FALSE 0

#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)


#endif /* __DEFINITIONS_H__ */

