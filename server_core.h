#ifndef SERVER_CORE_H
#define SERVER_CORE_H

struct state
{
    int client_socket;
    int server_socket;

    char *username;
    char *password;
};

void initialize(struct state *s_state);
void destroy(struct state *s_state);

int answer(struct state *s_state, int status_code, const char* message);

#endif

