#include <stdio.h>
#include <stdlib.h>
#include "commands.h"

void ftp_user(char ** parameters, struct state * s_state)
{
    printf("user not implemented yet.\n");
}

void ftp_pass(char ** parameters, struct state * s_state)
{
    printf("pass not implemented yet.\n");
}

void ftp_acct(char ** parameters, struct state * s_state)
{
    printf("acct not implemented yet.\n");
}

void ftp_cwd(char ** parameters, struct state * s_state)
{
    printf("cwd not implemented yet.\n");
}

void ftp_cdup(char ** parameters, struct state * s_state)
{
    printf("cdup not implemented yet.\n");
}

void ftp_smnt(char ** parameters, struct state * s_state)
{
    printf("smnt not implemented yet.\n");
}

void ftp_rein(char ** parameters, struct state * s_state)
{
    printf("rein not implemented yet.\n");
}

void ftp_quit(char ** parameters, struct state * s_state)
{
    printf("Quitting.\n");
    exit(0);
}

