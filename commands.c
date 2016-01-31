#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "commands.h"

char *trim_whitespace(char *string_in)
{
    while (isspace(string_in[0])) string_in++;

    if (string_in[0] == 0)
    {
        return string_in;
    }

    char *str_end = string_in + strlen(string_in) - 1;

    while (isspace(str_end[0])) str_end--;

    *(str_end + 1) = 0;

    return string_in;
}

void ftp_user(char *parameters, struct state *s_state)
{
    char *username = trim_whitespace(parameters);
    char msg_buf[256];

    sprintf(msg_buf, "Hello \"%s\"\n", username);
    write(s_state->client_socket , msg_buf , strlen(msg_buf));
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

