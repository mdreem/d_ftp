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

int main(int argc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in client;
    int listening_port = 8888;
    struct state s_state;

    initialize(&s_state);

    if (argc > 1)
    {
        listening_port = strtol(argv[1], NULL, 10);
    }

    socket_desc = initialize_socket(listening_port);

    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    new_socket =
        accept(socket_desc, (struct sockaddr *) &client,
               (socklen_t *) & c);
    if (new_socket < 0)
    {
        perror("accept failed");
        exit(1);
    }

    puts("Connection accepted");

    char *client_ip = inet_ntoa(client.sin_addr);
    int client_port = ntohs(client.sin_port);
    char msg_buf[1024];

    int connected = 1;
    char buffer[256];

    s_state.server_socket = socket_desc;
    s_state.client_socket = new_socket;

    char *current_dir = "../files";
    s_state.base_dir = malloc(sizeof(char) * (strlen(current_dir) + 1));
    memcpy(s_state.base_dir, current_dir, sizeof(current_dir));

    sprintf(msg_buf, "Hello %s:%d (%x)", client_ip, client_port,
            client.sin_addr);

    answer(&s_state, COMMAND_OKAY, msg_buf);

    chdir(s_state.base_dir);

    while (connected)
    {
        memset(buffer, 0, 256);
        int n = read(new_socket, buffer, 255);

        if (n < 0)
        {
            perror("error reading from socket");
        }

        printf("Länge: (%d; %d)\n", n, strlen(buffer));
        printf("Command: \"%s\"\n", trim_whitespace(buffer));

        parse(buffer, n, &s_state);
    }

    printf("Exiting connection-loop.");

    destroy(&s_state);

    return 0;
}
