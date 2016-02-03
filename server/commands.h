#ifndef COMMANDS_H
#define COMMANDS_H

#include "server_core.h"

char *trim_whitespace(char *string_in);
int isnewline(char c);

void ftp_user(char *, struct state *);
void ftp_pass(char *, struct state *);
void ftp_acct(char *, struct state *);
void ftp_cwd(char *, struct state *);
void ftp_cdup(char *, struct state *);
void ftp_smnt(char *, struct state *);
void ftp_rein(char *, struct state *);
void ftp_syst(char *, struct state *);
void ftp_quit(char *, struct state *);
void ftp_pasv(char *, struct state *);
void ftp_port(char *, struct state *);
void ftp_list(char *, struct state *);
void ftp_stru(char *, struct state *);
void ftp_mode(char *, struct state *);
void ftp_stor(char *, struct state *);
void ftp_retr(char *, struct state *);
void ftp_noop(char *, struct state *);
void ftp_debug(char *, struct state *);

typedef void (*command_function) (char *, struct state *);

struct command_to_function {
    command_function function;
    char name[64];
};

static const struct command_to_function commands[] = {
    {&ftp_user, "user"},
    {&ftp_pass, "pass"},
    {&ftp_acct, "acct"},
    {&ftp_cwd, "cwd"},
    {&ftp_cdup, "cdup"},
    {&ftp_smnt, "smnt"},
    {&ftp_rein, "rein"},
    {&ftp_syst, "syst"},
    {&ftp_quit, "quit"},
    {&ftp_pasv, "pasv"},
    {&ftp_port, "port"},
    {&ftp_list, "list"},
    {&ftp_stru, "stru"},
    {&ftp_mode, "mode"},
    {&ftp_stor, "stor"},
    {&ftp_retr, "retr"},
    {&ftp_noop, "noop"},
    {&ftp_debug, "debug"}
};

#endif
