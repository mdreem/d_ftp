#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h> 

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

void ftp_list(char *parameters, struct state *s_state)
{
    char buf[1024];
    char *buffer = "lalalalalaala\r\n";
    char *fin_buffer = "250 Fertig\r\n";
    answer(s_state, FILE_STATUS_OKAY, "Sending directory listing.");

    DIR *d;
    struct dirent *dir;
    //d = opendir(s_state->current_dir);
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            snprintf(buf, 1024, "%s\r\n", dir->d_name);
            write (s_state->active_socket, buf, strlen (buf));
        }

        closedir(d);
    }

    //write (s_state->active_socket, buffer, strlen (buffer));
    //write (s_state->active_socket, fin_buffer, strlen (fin_buffer));

    close(s_state->active_socket);
    answer(s_state, CLOSING_DATA_CONNECTION, "Finished sending directory listing.");
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

void ftp_port(char *parameters, struct state *s_state)
{
    printf("==PORT==\n");
    printf("Port: %s\n", parameters);

    struct ip ip;
    struct port port;

    close(s_state->active_socket);

    sscanf(parameters, "%d,%d,%d,%d,%d,%d", &ip.d1, &ip.d2, &ip.d3, &ip.d4, &port.d1, &port.d2);

    int ip_i, port_i;
    memcpy(&ip_i, &ip, sizeof(int));
    // TODO: Risky if int not zeroed?
    memcpy(&port_i, &port, sizeof(int));

    printf("Result: %x:%d\n", ip, port);

    int sock = socket (AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_addr.s_addr = ip_i;
    server.sin_family = AF_INET;
    server.sin_port = port_i;

    if (connect(sock ,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }

    s_state->active_socket = sock;
    answer(s_state, COMMAND_OKAY, "Port command succesful.");
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

void create_passive_connection(struct state *s_state)
{
    close(s_state->passive_socket);
    s_state->passive_socket = initialize_socket(12345);
}

void ftp_pasv(char *parameters, struct state *s_state)
{
    struct ip ip_addr;
    struct port port;
    char buf[256];
    socklen_t s_len = sizeof(struct sockaddr_in);

    printf("==PASV==\n");

    create_passive_connection(s_state);
    get_ip(s_state->passive_socket, &ip_addr);
    get_port(s_state->passive_socket, &port);

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
