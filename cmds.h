/*
 * cmds - sosh command module
*/
#ifndef __CMDS_H__
#define __CMDS_H__

#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "aux.h"


int cmd_quem();

int cmd_psu();

int cmd_usrbin(char *cmd);

int cmd_exit();

int cmd_localiza(char* cmd);

int cmd_ver();

int cmd_ajuda();
 
int cmd_hist();

int cmd_search_hist(char *cmd);

#endif /* __CMDS_H__ */

