#ifndef SERVER_CORE_H
#define SERVER_CORE_H

struct state
{
    int client_socket;
    int server_socket;

    char *username;
    char *password;
};

#endif

