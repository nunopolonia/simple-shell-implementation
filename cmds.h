/*
 * cmds - sosh command module
*/

#ifndef __CMDS_H__
#define __CMDS_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>




int cmd_quem();

int cmd_psu();

int cmd_usrbin(char** argv, int argc);

int cmd_exit();

int cmd_localiza();

int cmd_ver();

int cmd_ajuda();

#endif /* __CMDS_H__ */

