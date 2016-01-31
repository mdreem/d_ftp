#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include "commands.h"

int initialize_socket(int port)
{
    int socket_desc;
    struct sockaddr_in server;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket");
        exit(1);
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    puts("bind done");

    return socket_desc;
}

void parse(char *message, int maxlen)
{
    int command = 0;
    int pos = 0;

    char **parameters;

    struct state s_state;
    s_state.socket = 1;

    while(pos < maxlen) {
        while(isalpha(message[pos]))
        {
            message[pos] = tolower(message[pos]);
            pos = pos + 1;
            command += 1;
        }

        printf("Length of command: %d\n", command);

        pos += 1;
        if (message[pos] == 0)
        {
            break;
        }
    }

    int i;
    for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++)
    {
        if (strncmp(message, commands[i].name, strlen(commands[i].name)) == 0)
        {
            commands[i].function(parameters, &s_state);
        }
    }
}

int main(int argc , char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in client;

    socket_desc = initialize_socket(8888);

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket<0)
    {
        perror("accept failed");
        exit(1);
    }

    puts("Connection accepted");

    char *client_ip = inet_ntoa(client.sin_addr);
    int client_port = ntohs(client.sin_port);
    char msg_buf[1024];

    sprintf(msg_buf, "Hello %s:%d\n", client_ip, client_port);

    write(new_socket , msg_buf , strlen(msg_buf));

    int connected = 1;
    char buffer[256];

    while(connected)
    {
        memset(buffer, 0, 256);
        int n = read(new_socket, buffer, 255);

        if (n < 0)
        {
            perror("error reading from socket");
        }

        printf("Länge: %d\n", strlen(buffer));
        printf("\"%s\"", buffer);
        fflush(stdout);

        parse(buffer, 256);

        sprintf(msg_buf, "Hey %s:%d I received your message\n", client_ip, client_port);
        write(new_socket , msg_buf , strlen(msg_buf));
    }

    return 0;
}
