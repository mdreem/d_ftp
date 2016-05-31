#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "status_codes.h"
#include "server_core.h"
#include "commands.h"
#include "tools.h"

void initialize(struct state *s_state)
{
    s_state->client_socket = 0;
    s_state->server_socket = 0;
    s_state->passive_socket = 0;
    s_state->active_socket = 0;
    s_state->username = NULL;
    s_state->password = NULL;
    s_state->base_dir = NULL;
    s_state->s_state = SERVER_STANDARD_MODE;
}

void destroy(struct state *s_state)
{
    if (s_state->username != NULL)
    {
        free(s_state->username);
    }

    if (s_state->password != NULL)
    {
        free(s_state->password);
    }

    if (s_state->base_dir != NULL)
    {
        free(s_state->base_dir);
    }
}

void parse(char *message, int maxlen, struct state *s_state)
{
    int command = 0, command_length;
    int param_pos;
    int pos = 0;

    char *parameters;

    while (isspace(message[pos]) && pos < maxlen)
    {
        pos += 1;
        command += 1;
    }

    while (isalpha(message[pos]))
    {
        message[pos] = tolower(message[pos]);
        pos += +1;
    }
    command_length = pos - command;

    param_pos = pos;
    while (isspace(message[pos]) && pos < maxlen)
    {
        pos += 1;
        param_pos += 1;
    }

    parameters = message + param_pos;
    printf("Parameters (%d + %d): \"%s\"\n", command, param_pos,
           parameters);
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        int len = strlen(commands[i].name);
        if (command + len >= maxlen)
            continue;

        if (strncmp(message + command, commands[i].name, len) == 0)
        {
            commands[i].function(parameters, s_state);
            return;
        }
    }
    answer(s_state, NOT_IMPLEMENTED, "Command not implemented.");
}
