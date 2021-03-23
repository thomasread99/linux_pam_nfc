/*Headers to include*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

#include "../include/authenticate.h"
#include "../include/nfc_functions.h"

static struct pam_conv conv = {
    misc_conv,
    NULL
};

int main (int argc, char *argv[])
{
    /*Run the function to authenticate the user through PAM*/
    int granted = pamAuthenticate (argc, argv);

    if (granted == 0) {
        printf("Authorised\n");
        exit(EXIT_SUCCESS);
    }

    printf("Unauthorised\n");
    exit(EXIT_FAILURE);
}

/*Function to authenticate through PAM*/
PAM_EXTERN
int pamAuthenticate (int argc, const char **argv)
{
    /*Variable declarations*/
    pam_handle_t *pamh = NULL;
    int retval = PAM_AUTH_ERR;
    char confline[256];
    const char *user = NULL;

    /*Begin the PAM transaction*/
    pam_start(CONFIG, NULL, &conv, &pamh);

    /*Get the user name through the command line*/
    retval = pam_get_user (pamh, &user, NULL);

    /*Check that the user name has been retrieved*/
    if (retval != PAM_SUCCESS)
    {
        printf ("User not found\n");
        return retval;
    }
    if (user == NULL || *user == '\0')
    {
        printf("Username not known\n");
        return retval;
    }

    /*Check that the config file exists and can be opened*/
    if (!(checkFile())) {
        return PAM_SERVICE_ERR;
    }

    /*Check if the user can be authenticated*/
    return (authorise (user)) ? PAM_SUCCESS : PAM_AUTH_ERR;
}