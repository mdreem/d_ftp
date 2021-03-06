#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <arpa/inet.h>

struct state {
    int client_socket;
    int server_socket;
    int passive_socket;
    int active_socket;

    char *username;
    char *password;

    char *base_dir;

    int s_state;
};

enum server_state {
    SERVER_PASSIVE_MODE, SERVER_STANDARD_MODE
};

struct ip {
    unsigned char d1;
    unsigned char d2;
    unsigned char d3;
    unsigned char d4;
};

struct port {
    unsigned char d1;
    unsigned char d2;
};

void initialize(struct state *s_state);
void destroy(struct state *s_state);

#endif
