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
    int len = strlen(username);

    if (s_state->username != NULL )
    {
        free (s_state->username);
    }

    s_state->username = (char *) malloc(sizeof(char) * (len + 1));
    memset (s_state->username, 0, len + 1);
    strncpy(s_state->username, username, len);

    snprintf(msg_buf, 256, "Hello \"%s\"\n", s_state->username);
    answer(s_state, 230, msg_buf);
}

void ftp_pass(char *parameters, struct state *s_state)
{
    char *password = trim_whitespace(parameters);
    char msg_buf[256];
    int len = strlen(password);

    if (s_state->password != NULL )
    {
        free (s_state->password);
    }

    s_state->password = (char *) malloc(sizeof(char) * (len + 1));
    memset (s_state->password, 0, len + 1);
    strncpy(s_state->password, password, len);

    snprintf(msg_buf, 256, "200 Hello %s\n", s_state->password);
    answer(s_state, 230, msg_buf);
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

void ftp_syst(char *parameters, struct state *s_state)
{
    answer(s_state, 215, "Linux");
}

void ftp_quit(char *parameters, struct state *s_state)
{
    printf("Quitting.\n");
    destroy(s_state);
    exit(0);
}

void ftp_debug(char *parameters, struct state *s_state)
{
    char debug_information[1024];
    sprintf (debug_information, "==STATE==\nServer Socket Descriptor: %d\nClient Socket Descriptor: %d\nUsername: %s\nPassword: %s\n",
            s_state->server_socket,
            s_state->client_socket,
            s_state->username,
            s_state->password);

    write(s_state->client_socket , debug_information , strlen(debug_information));
    printf("%s", debug_information);
}
