/*
 * cmds - sosh command module
*/

#ifndef __CMDS_H__
#define __CMDS_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
 
#include "aux.h"

int cmd_quem();

int cmd_psu();

int cmd_usrbin(char** cmd);

int cmd_exit();

int cmd_localiza(char** cmd);

int cmd_ver();

int cmd_ajuda();

int cmd_hist();

#endif /* __CMDS_H__ */

