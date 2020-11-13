/*Need to include
     # check authorization
    auth       required     pam_unix.so
    account    required     pam_unix.so
  in whichever config file is used - not currently working 
  **UPDATE**
  Program seems to work fine when nothing is in the specified
  config file, and the constant MY_CONFIG can be any value */

/*Include the relevant header files*/
#include "pam_appl.h"
#include "pam_misc.h"
#include <stdio.h>

/*Define which config file will be used*/
#define MY_CONFIG "login"

static struct pam_conv conv = {
    misc_conv,
    NULL
};

int main(int argc, char *argv[])
{
    pam_handle_t *pamh = NULL;
    int retval;
    const char *user = "nobody";

    if (argc == 2) {
        user = argv[1];
    }
    if (argc > 2) {
        fprintf(stderr, "Usage: check_user [username]\n");
        exit(1);
    }

    /*Create the PAM context and initiate the transaction*/
    retval = pam_start(MY_CONFIG, user, &conv, &pamh);

    if (retval == PAM_SUCCESS) {        
        /*Authenticate the user*/
        retval = pam_authenticate(pamh,0);
    }

    if (retval == PAM_SUCCESS) {        
        /*Determine if their account is valid*/
        retval = pam_acct_mgmt(pamh, 0);
    }

    /*Authenticated or not*/
    if (retval == PAM_SUCCESS) {
        fprintf(stdout, "Authenticated\n");
    } else {
        fprintf(stdout, "Not Authenticated\n");
    }

    /*Terminate the PAM transaction*/
    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        fprintf(stderr, "check_user: failed to release authenticator\n");
        exit(1);
    }

    /*Indicate Success*/
    return (retval == PAM_SUCCESS ? 0:1);
}