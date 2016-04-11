#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>

#include "tools.h"
#include "commands.h"
#include "status_codes.h"
#include "auth.h"

void ftp_user(char *parameters, struct state *s_state)
{
    char *username = trim_whitespace(parameters);
    char msg_buf[256];
    int len = strlen(username);

    printf("==USER==\n");

    if (s_state->username != NULL)
    {
        free(s_state->username);
    }

    s_state->username = (char *) malloc(sizeof(char) * (len + 1));
    memset(s_state->username, 0, len + 1);
    strncpy(s_state->username, username, len);

    snprintf(msg_buf, 256, "Hello \"%s\"", s_state->username);
    answer(s_state, NEED_PASSWORD, msg_buf);
}

void ftp_pass(char *parameters, struct state *s_state)
{
    char *password = trim_whitespace(parameters);
    char msg_buf[256];
    int len = strlen(password);

    printf("==PASS==\n");

    if (s_state->password != NULL)
    {
        free(s_state->password);
    }

    s_state->password = (char *) malloc(sizeof(char) * (len + 1));
    memset(s_state->password, 0, len + 1);
    strncpy(s_state->password, password, len);


    int res = authenticate(s_state->username, s_state->password);

    if (res == DFTP_AUTH_SUCCESS)
    {
        snprintf(msg_buf, 256, "Logged in. given: %s", s_state->password);
        answer(s_state, USER_LOGGED_IN, msg_buf);
    }
    else
    {
        snprintf(msg_buf, 256, "Not logged in. Given: %s", s_state->password);
        answer(s_state, NOT_LOGGED_IN, msg_buf);
    }
}

void ftp_acct(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "acct not implemented yet.");
}

void ftp_stor(char *parameters, struct state *s_state)
{
    char buf[256];
    int n = 0;
    FILE *dest_file;
    int sock;

    printf("==STOR==\n");
    answer(s_state, FILE_STATUS_OKAY, "STOR file status okay.");

    sock = get_socket(s_state);

    dest_file = fopen(trim_whitespace(parameters), "wb");

    do
    {
        memset(buf, 0, 256);
        n = read(sock, buf, 255);
        fwrite(buf, 1, n, dest_file);
    }
    while (n > 0);

    answer(s_state, CLOSING_DATA_CONNECTION, "File transfer succesful.");
    close(sock);
    s_state->active_socket = -1;
    s_state->passive_socket = -1;
    fclose(dest_file);
}

void ftp_retr(char *parameters, struct state *s_state)
{
    char buf[256];
    int n = 0;
    FILE *source_file;
    int sock;

    printf("==RETR==\n");
    answer(s_state, FILE_STATUS_OKAY, "RETR.");

    sock = get_socket(s_state);

    source_file = fopen(trim_whitespace(parameters), "rb");

    do
    {
        n = fread(buf, 1, 256, source_file);
        write(sock, buf, n);
    }
    while (n > 0);

    answer(s_state, CLOSING_DATA_CONNECTION, "File transfer succesful.");
    close(sock);
    s_state->active_socket = -1;
    s_state->passive_socket = -1;
    fclose(source_file);
}

void ftp_list(char *parameters, struct state *s_state)
{
    char buf[1024];
    char cwd[1024];
    int sock;
    answer(s_state, FILE_STATUS_OKAY, "Sending directory listing.");

    printf("Sending contents of %s\n", s_state->base_dir);

    sock = get_socket(s_state);

    DIR *d;
    struct dirent *dir;

    getcwd(cwd, sizeof(cwd));
    d = opendir(cwd);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            snprintf(buf, 1024, "%s\r\n", dir->d_name);
            write(sock, buf, strlen(buf));
        }

        closedir(d);
    }

    close(sock);
    s_state->active_socket = -1;
    s_state->passive_socket = -1;
    s_state->s_state = SERVER_STANDARD_MODE;
    answer(s_state, CLOSING_DATA_CONNECTION,
           "Finished sending directory listing.");
}

void ftp_smnt(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "smnt not implemented yet.");
}

void ftp_rein(char *parameters, struct state *s_state)
{
    answer(s_state, NOT_IMPLEMENTED, "rein not implemented yet.");
}

void ftp_syst(char *parameters, struct state *s_state)
{
    answer(s_state, SYSTEM_TYPE, "Linux");
}

void ftp_port(char *parameters, struct state *s_state)
{
    printf("==PORT==\n");
    printf("Port: %s\n", parameters);

    struct ip ip;
    struct port port;

    close(s_state->active_socket);

    sscanf(parameters, "%d,%d,%d,%d,%d,%d", &ip.d1, &ip.d2, &ip.d3, &ip.d4,
           &port.d1, &port.d2);

    int ip_i, port_i;
    memcpy(&ip_i, &ip, sizeof(int));
    // TODO: Risky if int not zeroed?
    memcpy(&port_i, &port, sizeof(int));

    printf("Result: %x:%d\n", ip, port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_addr.s_addr = ip_i;
    server.sin_family = AF_INET;
    server.sin_port = port_i;

    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }

    s_state->active_socket = sock;
    answer(s_state, COMMAND_OKAY, "Port command succesful.");
}

void ftp_quit(char *parameters, struct state *s_state)
{
    printf("Quitting.\n");
    destroy(s_state);
    exit(0);
}

void ftp_stru(char *parameters, struct state *s_state)
{
    printf("==STRU==\n");
    printf("paremeter: %s", parameters);
    answer(s_state, COMMAND_OKAY, "STRU command succesful.");
}

void ftp_mode(char *parameters, struct state *s_state)
{
    printf("==MODE==\n");
    printf("paremeter: %s", parameters);
    answer(s_state, COMMAND_OKAY, "MODE command succesful.");
}

void ftp_noop(char *parameters, struct state *s_state)
{
    answer(s_state, COMMAND_OKAY, "NOOP okay.");
}

void ftp_cwd(char *parameters, struct state *s_state)
{
    chdir(parameters);
    answer(s_state, REQUESTED_FILE_ACTION_OKAY, "Changed directory.");
}

void ftp_cdup(char *parameters, struct state *s_state)
{
    chdir("..");
    answer(s_state, COMMAND_OKAY, "Changed directory.");
}

void ftp_pasv(char *parameters, struct state *s_state)
{
    struct ip ip_addr;
    struct port port;
    char buf[256];
    socklen_t s_len = sizeof(struct sockaddr_in);

    printf("==PASV==\n");

    create_passive_connection(s_state);
    get_ip(s_state->passive_socket, &ip_addr);
    get_port(s_state->passive_socket, &port);

    listen(s_state->passive_socket, 3);
    s_state->s_state = PASSIVE_MODE;
    snprintf(buf, 256, "Entering passive mode (%d,%d,%d,%d,%d,%d).",
             ip_addr.d1, ip_addr.d2, ip_addr.d3, ip_addr.d4, port.d1,
             port.d2);
    answer(s_state, PASSIVE_MODE, buf);
}

void ftp_debug(char *parameters, struct state *s_state)
{
    char debug_information[1024];
    sprintf(debug_information,
            "==STATE==\nServer Socket Descriptor: %d\nClient Socket Descriptor: %d\nUsername: %s\nPassword: %s\n",
            s_state->server_socket, s_state->client_socket,
            s_state->username, s_state->password);

    answer(s_state, COMMAND_OKAY, debug_information);
    printf("%s", debug_information);
}
