#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "commands.h"

    int
initialize_socket (int port)
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

    return socket_desc;
}

    void
parse (char *message, int maxlen, struct state *s_state)
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

    /*
    while (pos < maxlen)
    {
        printf ("Length of command: %d\n", command);

        while (isspace (message[pos]) && pos < maxlen)
        {
            param_pos += 1;
            pos += 1;
        }

        if (message[pos] == 0)
        {
            break;
        }
    }
    */

    parameters = message + param_pos;
    printf ("Parameters (%d + %d): \"%s\"", command, param_pos, parameters);
    for (int i = 0; i < sizeof (commands) / sizeof (commands[0]); i++)
    {
        int len = strlen(commands[i].name);
        if (command + len >= maxlen) continue;

        if (strncmp (message + command, commands[i].name, len) == 0)
        {
            commands[i].function (parameters, s_state);
        }
    }
}

    int
main (int argc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in client;
    int listening_port = 8888;

    if (argc > 1)
    {
        listening_port = strtol (argv[1], NULL, 10);
    }

    socket_desc = initialize_socket (listening_port);

    listen (socket_desc, 3);

    puts ("Waiting for incoming connections...");
    c = sizeof (struct sockaddr_in);
    new_socket =
        accept (socket_desc, (struct sockaddr *) &client, (socklen_t *) & c);
    if (new_socket < 0)
    {
        perror ("accept failed");
        exit (1);
    }

    puts ("Connection accepted");

    char *client_ip = inet_ntoa (client.sin_addr);
    int client_port = ntohs (client.sin_port);
    char msg_buf[1024];

    sprintf (msg_buf, "Hello %s:%d\n", client_ip, client_port);

    write (new_socket, msg_buf, strlen (msg_buf));

    int connected = 1;
    char buffer[256];

    struct state s_state;
    s_state.server_socket = socket_desc;
    s_state.client_socket = new_socket;

    while (connected)
    {
        memset (buffer, 0, 256);
        int n = read (new_socket, buffer, 255);

        if (n < 0)
        {
            perror ("error reading from socket");
        }

        printf ("Länge: %d\n", strlen (buffer));
        printf ("\"%s\"\n", buffer);
        fflush (stdout);

        parse (buffer, 256, &s_state);

        sprintf (msg_buf, "Hey %s:%d I received your message\n", client_ip,
                client_port);
        write (new_socket, msg_buf, strlen (msg_buf));
    }

    return 0;
}
