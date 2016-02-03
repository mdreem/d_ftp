#include <string.h>

#include "tools.h"

int initialize_socket(int port)
{
    int socket_desc;
    struct sockaddr_in server;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    printf("bind done. port: %d\n", port);

    return socket_desc;
}


int answer(struct state *s_state, int status_code, const char *message)
{
    char buffer[256];
    snprintf(buffer, 256, "%d %s\r\n", status_code, message);

    int n = write(s_state->client_socket, buffer, strlen(buffer));

    for (int i = 0; i < strlen(buffer) && i < 256; i++)
    {
        if (isnewline(buffer[i]))
            buffer[i] = '@';
    }
    printf("Answered: \"%s\"\n", buffer);
    return n;
}


int isnewline(char c)
{
    return c == '\n' || c == '\r';
}

char *trim_whitespace(char *string_in)
{
    while (isspace(string_in[0]))
        string_in++;

    if (string_in[0] == 0)
    {
        return string_in;
    }

    char *str_end = string_in + strlen(string_in) - 1;

    while (isspace(str_end[0]) || isnewline(str_end[0]))
        str_end--;

    *(str_end + 1) = 0;

    return string_in;
}

int get_socket(struct state *s_state)
{
    if (s_state->s_state == SERVER_STANDARD_MODE)
    {
        return s_state->active_socket;
    }
    else
    {
        struct sockaddr_in client;
        int c = sizeof(struct sockaddr_in);
        return accept(s_state->passive_socket,
                      (struct sockaddr *) &client, (socklen_t *) & c);
    }
}

void get_sockaddr(int socket, struct sockaddr_in *addr)
{
    socklen_t s_len = sizeof(struct sockaddr_in);
    getsockname(socket, (struct sockaddr *) addr, &s_len);
}

void get_ip(int socket, struct ip *ip_addr)
{
    struct sockaddr_in addr;
    get_sockaddr(socket, &addr);
    memcpy(ip_addr, &addr.sin_addr, sizeof(struct ip));
}

void get_port(int socket, struct port *port)
{
    struct sockaddr_in addr;
    get_sockaddr(socket, &addr);
    memcpy(port, &addr.sin_port, sizeof(struct port));
}

void create_passive_connection(struct state *s_state)
{
    close(s_state->passive_socket);
    s_state->passive_socket = initialize_socket(0);
}
