#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"

void ftp_user(char *parameters, struct state *s_state)
{
    char msg_buf[256];
    sprintf(msg_buf, "Hello \"%s\"\n", parameters);
    write(s_state->client_socket , msg_buf , strlen(msg_buf));
    printf("Hello \"%s\"\n", parameters);
    //printf("user not implemented yet.\n");
}

void ftp_pass(char *parameters, struct state *s_state)
{
    printf("pass not implemented yet.\n");
}

void ftp_acct(char *parameters, struct state *s_state)
{
    printf("acct not implemented yet.\n");
}

void ftp_cwd(char *parameters, struct state *s_state)
{
    printf("cwd not implemented yet.\n");
}

void ftp_cdup(char *parameters, struct state *s_state)
{
    printf("cdup not implemented yet.\n");
}

void ftp_smnt(char *parameters, struct state *s_state)
{
    printf("smnt not implemented yet.\n");
}

void ftp_rein(char *parameters, struct state *s_state)
{
    printf("rein not implemented yet.\n");
}

void ftp_quit(char *parameters, struct state *s_state)
{
    printf("Quitting.\n");
    exit(0);
}

