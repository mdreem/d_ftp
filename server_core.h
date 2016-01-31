#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <arpa/inet.h>

struct state
{
    int client_socket;
    int server_socket;
    int passive_socket;

    char *username;
    char *password;

    struct sockaddr_in server;
    struct sockaddr_in client;

    char *current_dir;
};

struct ip
{
    unsigned char d1;
    unsigned char d2;
    unsigned char d3;
    unsigned char d4;
};

struct port
{
    unsigned char d1;
    unsigned char d2;
};

void initialize(struct state *s_state);
void destroy(struct state *s_state);

int answer(struct state *s_state, int status_code, const char* message);

#endif

