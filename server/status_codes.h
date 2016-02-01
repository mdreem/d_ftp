#ifndef STATUS_CODES_H
#define STATUS_CODES_H

enum
{
    DATA_CONNECTION_ALREADY_OPEN = 125,
    FILE_STATUS_OKAY = 150,
    COMMAND_OKAY = 200,
    SYSTEM_TYPE = 215,
    CLOSING_DATA_CONNECTION = 226,
    PASSIVE_MODE = 227,
    USER_LOGGED_IN = 230,
    REQUESTED_FILE_ACTION_OKAY = 250,
    NEED_PASSWORD = 331,
    SERVICE_NOT_AVAILABLE = 421,
    SYNTAX_ERROR = 500,
    NOT_IMPLEMENTED = 502,
    NOT_LOGGED_IN = 530
};

#endif