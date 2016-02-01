#ifndef TOOLS_H
#define TOOLS_H

#include "server_core.h"

int initialize_socket (int port);
int answer(struct state *s_state, int status_code, const char* message);
int isnewline(char c);
char *trim_whitespace(char *string_in);
int get_socket(struct state *s_state);
void get_sockaddr(int socket, struct sockaddr_in* addr);
void get_ip(int socket, struct ip *ip_addr);
void get_port(int socket, struct port *port);
void create_passive_connection(struct state *s_state);

#endif
