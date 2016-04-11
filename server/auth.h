#ifndef AUTH_H
#define AUTH_H

int authenticate(char* username, char* password);

enum { DFTP_AUTH_SUCCESS, DFTP_AUTH_USER_UNKNOWN, DFTP_AUTH_ERR };

#endif
