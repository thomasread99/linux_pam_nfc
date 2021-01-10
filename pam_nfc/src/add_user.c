/*Headers to include*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#include "../include/add_user.h"
#include "../include/nfc_functions.h"

int main (int argc, char *argv[])
{
    /*Check that the user has supplied their username as a parameter*/
    if (argc != 2)
    {
        fprintf(stderr, "Add your username as a parameter\n");
        exit(EXIT_FAILURE);
    }

    /*Variable declarations*/
    char **targets;
    int n;

    /*Set n to the number of NFC targets*/
    n = getTargets (&targets);

    /*Check that one and only one target is detected*/
    if (n != 1) {
        errx (EXIT_FAILURE, "%d targets detected", n);
    } 

    /*Run the function to add the user to the configuration file and error check*/
    if (!(addAuth (argv[1], targets[0]))) {
        err (EXIT_FAILURE, "Error adding authorization for the user");
    }

    /*Free the memory*/
    free (targets[0]);
	free (targets);

    /*Exit and report success*/
    printf("User added\n");
    exit(EXIT_SUCCESS);
}

/*Add the user and tag to the configuration file*/
int addAuth (char *user, char *target)
{
    /*Variable declarations*/
    int ret;
    FILE *config;

    /*If the config gile exists, check that it is read-only*/
    if ((config = fopen (CONFIGFILE, "r"))) {
        fclose (config);
        /*Chmod the file so it can be written to*/
        chmod (CONFIGFILE, 0600);
    }

    /*If no file exists, avoid race condition*/
    umask (0077);

    /*If you can't append to the file, return*/
    if (!(config = fopen (CONFIGFILE, "a"))) {
        return 0;
    }

    /*Write to the config file*/
    ret = (fprintf (config, "%s %s\n", user, crypt(target, "RC")) > 0);

    /*If you can't close the config file, return*/
    if (fclose (config) != 0) {
        return 0;
    }

    /*Set the file back to read-only*/
    chmod (CONFIGFILE, 0400);

    /*Return*/
    return ret;
}