#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "commands.h"
#include "status_codes.h"

int isnewline(char c)
{
    return c == '\n' || c == '\r';
}

char *trim_whitespace(char *string_in)
{
    while (isspace(string_in[0])) string_in++;

    if (string_in[0] == 0)
    {
        return string_in;
    }

    char *str_end = string_in + strlen(string_in) - 1;

    while (isspace(str_end[0]) || isnewline(str_end[0])) str_end--;

    *(str_end + 1) = 0;

    return string_in;
}

void ftp_user(char *parameters, struct state *s_state)
{
    char *username = trim_whitespace(parameters);
    char msg_buf[256];
    int len = strlen(username);

    printf("==USER==\n");

    if (s_state->username != NULL )
    {
        free (s_state->username);
    }

    s_state->username = (char *) malloc(sizeof(char) * (len + 1));
    memset (s_state->username, 0, len + 1);
    strncpy(s_state->username, username, len);

    snprintf(msg_buf, 256, "Hello \"%s\"", s_state->username);
    answer(s_state, NEED_PASSWORD, msg_buf);
}

void ftp_pass(char *parameters, struct state *s_state)
{
    char *password = trim_whitespace(parameters);
    char msg_buf[256];
    int len = strlen(password);

    printf("==PASS==\n");

    if (s_state->password != NULL )
    {
        free (s_state->password);
    }

    s_state->password = (char *) malloc(sizeof(char) * (len + 1));
    memset (s_state->password, 0, len + 1);
    strncpy(s_state->password, password, len);

    snprintf(msg_buf, 256, "Given: %s", s_state->password);
    answer(s_state, USER_LOGGED_IN, msg_buf);
}

void ftp_acct(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "acct not implemented yet.");
}

void ftp_cwd(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "cwd not implemented yet.");
}

void ftp_cdup(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "cdup not implemented yet.");
}

void ftp_smnt(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "smnt not implemented yet.");
}

void ftp_rein(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "rein not implemented yet.");
}

void ftp_syst(char *parameters, struct state *s_state)
{
    answer(s_state, SYSTEM_TYPE, "Linux");
}

void ftp_quit(char *parameters, struct state *s_state)
{
    printf("Quitting.\n");
    destroy(s_state);
    exit(0);
}

void get_sockaddr(int socket, struct sockaddr_in* addr)
{
    socklen_t s_len = sizeof(struct sockaddr_in);
    getsockname(socket, (struct sockaddr *)addr, &s_len);
}

void get_ip(int socket, struct ip *ip_addr)
{
    struct sockaddr_in addr;
    get_sockaddr(socket, &addr);
    memcpy (ip_addr, &addr.sin_addr, sizeof(struct ip));
}

void get_port(int socket, struct port *port)
{
    struct sockaddr_in addr;
    get_sockaddr(socket, &addr);
    memcpy (port, &addr.sin_port, sizeof(struct port));
}

void ftp_pasv(char *parameters, struct state *s_state)
{
    struct ip ip_addr;
    struct port port;
    char buf[256];
    socklen_t s_len = sizeof(struct sockaddr_in);

    printf("==PASV==\n");

    struct sockaddr_in addr;
    getsockname(s_state->server_socket, (struct sockaddr *)&addr, &s_len);

    //memcpy (&ip_addr, &addr.sin_addr, sizeof(struct ip));
    //memcpy (&port_p, &addr.sin_port, sizeof(struct port));

    get_ip(s_state->server_socket, &ip_addr);
    get_port(s_state->server_socket, &port);

    printf("Ip: %d\n", addr.sin_addr);
    printf("Port: %d\n", addr.sin_port);
    printf("Ip: %d\n", s_state->server.sin_addr);
    printf("Port: %d\n", s_state->server.sin_port);

    snprintf(buf, 256, "Entering passive mode (%d,%d,%d,%d,%d,%d).", ip_addr.d1, ip_addr.d2, ip_addr.d3, ip_addr.d4, port.d1, port.d2 );
    answer(s_state, PASSIVE_MODE, buf);
}

void ftp_debug(char *parameters, struct state *s_state)
{
    char debug_information[1024];
    sprintf (debug_information, "==STATE==\nServer Socket Descriptor: %d\nClient Socket Descriptor: %d\nUsername: %s\nPassword: %s\n",
            s_state->server_socket,
            s_state->client_socket,
            s_state->username,
            s_state->password);

    answer(s_state, COMMAND_OKAY, debug_information);
    printf("%s", debug_information);
}
