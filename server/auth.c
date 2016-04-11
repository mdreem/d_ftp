#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>

#include "auth.h"

struct pam_response *reply;

int conversation(int num_msg, const struct pam_message **msg,
            struct pam_response **response, void *appdata_ptr)
{
    *response = reply;
    return PAM_SUCCESS;
}

int authenticate(char* username, char* password)
{
    static struct pam_conv pam_conversation = { conversation, NULL };
    pam_handle_t* pamh;
    int return_code;

    reply = (struct pam_response *)malloc(sizeof(struct pam_response));

    reply[0].resp = strdup(password);
    reply[0].resp_retcode = 0;

    int res = pam_start("d_ftp", username, &pam_conversation, &pamh);

    if (res != PAM_SUCCESS)
    {
        return -1;
    }

    res = pam_authenticate(pamh, 0);

    printf("Result: %d\n", res);

    if (res == PAM_SUCCESS)
    {
        printf("The user was successfully authenticated\n");
        return_code = DFTP_AUTH_SUCCESS;
    }
    else if (res == PAM_USER_UNKNOWN)
    {
        printf("Unknown user\n");
        return_code = DFTP_AUTH_USER_UNKNOWN;
    }
    else
    {
        printf("Authentication failed\n");
        return_code = DFTP_AUTH_ERR;
    }

    res = pam_end(pamh, res);
    return return_code;
}
