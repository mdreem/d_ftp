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

int answer(struct state *s_state, int status_code, const char* message)
{
    char buffer[256];
    snprintf(buffer, 256, "%d %s\n", status_code, message);

    int n = write (s_state->client_socket, buffer, strlen (buffer));

    for (int i=0; i < strlen(buffer) && i < 256; i++)
    {
        if(isnewline(buffer[i])) buffer[i] = '@';
    }
    printf("Answered: \"%s\"\n", buffer);
    return n;
}

void initialize(struct state *s_state)
{
    s_state->client_socket = 0;
    s_state->server_socket = 0;
    s_state->username = NULL;
    s_state->password = NULL;
    s_state->current_dir = NULL;
}

void destroy(struct state *s_state)
{
    if (s_state->username != NULL)
    {
        free (s_state->username);
    }

    if (s_state->password != NULL)
    {
        free (s_state->password);
    }

    if (s_state->current_dir != NULL)
    {
        free (s_state->current_dir);
    }
}

int initialize_socket (int port, struct state *s_state)
{
    int socket_desc;
    struct sockaddr_in server;

    socket_desc = socket (AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        perror ("Could not create socket");
        exit (1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons (port);

    if (bind (socket_desc, (struct sockaddr *) &server, sizeof (server)) < 0)
    {
        perror ("bind failed");
        exit (1);
    }
    printf ("bind done. port: %d\n", port);

    memcpy(&s_state->server, &server, sizeof(struct sockaddr_in));
    return socket_desc;
}

void parse(char *message, int maxlen, struct state *s_state)
{
    int command = 0, command_length;
    int param_pos;
    int pos = 0;

    char *parameters;

    while (isspace (message[pos]) && pos < maxlen)
    {
        pos += 1;
        command += 1;
    }

    while (isalpha (message[pos]))
    {
        message[pos] = tolower (message[pos]);
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
    printf ("Parameters (%d + %d): \"%s\"\n", command, param_pos, parameters);
    for (int i = 0; i < sizeof (commands) / sizeof (commands[0]); i++)
    {
        int len = strlen(commands[i].name);
        if (command + len >= maxlen) continue;

        if (strncmp (message + command, commands[i].name, len) == 0)
        {
            commands[i].function (parameters, s_state);
            return;
        }
    }
    answer(s_state, NOT_IMPLEMENTED, "Command not implemented.");
}

int main (int argc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in client;
    int listening_port = 8888;
    struct state s_state;

    initialize(&s_state);

    if (argc > 1)
    {
        listening_port = strtol (argv[1], NULL, 10);
    }

    socket_desc = initialize_socket (listening_port, &s_state);

    listen (socket_desc, 3);

    puts ("Waiting for incoming connections...");
    c = sizeof (struct sockaddr_in);
    new_socket = accept (socket_desc, (struct sockaddr *) &client, (socklen_t *) & c);
    if (new_socket < 0)
    {
        perror ("accept failed");
        exit (1);
    }

    puts ("Connection accepted");

    memcpy(&(s_state.client), &client, sizeof(struct sockaddr_in));

    char *client_ip = inet_ntoa (client.sin_addr);
    int client_port = ntohs (client.sin_port);
    char msg_buf[1024];

    int connected = 1;
    char buffer[256];

    s_state.server_socket = socket_desc;
    s_state.client_socket = new_socket;

    char *current_dir = "files";
    s_state.current_dir = malloc(sizeof(char) * (strlen(current_dir) + 1));

    sprintf (msg_buf, "Hello %s:%d (%x)", client_ip, client_port, client.sin_addr);

    answer(&s_state, COMMAND_OKAY, msg_buf);

    while (connected)
    {
        memset (buffer, 0, 256);
        int n = read (new_socket, buffer, 255);

        if (n < 0)
        {
            perror ("error reading from socket");
        }

        printf ("Länge: (%d; %d)\n", n, strlen (buffer));
        printf ("Command: \"%s\"\n", trim_whitespace(buffer));

        parse (buffer, n, &s_state);
    }

    printf ("Exiting connection-loop.");

    destroy(&s_state);

    return 0;
}
