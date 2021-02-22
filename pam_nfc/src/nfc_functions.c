/*Headers to include*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nfc/nfc.h>
#include <sys/stat.h>

#include "../include/add_user.h"
#include "../include/nfc_functions.h"
#include "../include/apdu_functions.h"

/*Function to check if the user is authorised with their tag*/
int authorise (const char *user)
{
    /*Initially set to unauthorised*/
    int authorised = 0;

    /*Variable declarations*/
    char *id;

    /*Set the id to the id received from the device*/
    id = getID();

    /*Check that an ID has been received*/
    if (id == NULL) {
        errx (EXIT_FAILURE, "Error communicating with device\n");
    } 

    /*Check if the user is authorized with the id received*/
    if (checkAuth(user, id)) {
        authorised = 1;        
    }
    
    return authorised;
}

/*Function to check if the user and tag are found in the config*/
int checkAuth (const char *user, char *id)
{
    /*Initially set to not found*/
    int found = 0;

    /*Variable declarations*/
    FILE *config;
    char needle[BUFSIZ];

    /*Write to the buffer*/
    snprintf (needle, BUFSIZ, "%s %s\n", user, crypt(id, "RC"));

    /*Open the config file for reading*/
    if ((config = fopen(CONFIGFILE, "r"))) {
        char buffer[BUFSIZ];
        /*Look at each line in the file*/
        while (!found && fgets (buffer, BUFSIZ, config)) {
            /*If the value matches the value placed in the buffer, then user found*/
            if (strcmp (buffer, needle) == 0) {
                found = 1;
            }
        }
        /*Close the file*/
        fclose (config);
    }

    /*Return the result*/
    return found;
}

/*Check that the config file exists and can be read*/
int checkFile (void) 
{
    /*Variable declarations*/
    struct stat fileInfo;

    /*Check file exists*/
    if (stat (CONFIGFILE, &fileInfo)) {
        return 0;
    }

    /*Check file can be opened*/
    if ((fileInfo.st_mode & S_IWOTH)
            || !S_ISREG (fileInfo.st_mode))
    {
        return 0;
    }

    return 1;
}