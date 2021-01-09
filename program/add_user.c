/*Headers to include*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nfc/nfc.h>
#include <sys/stat.h>

#include "add_user.h"

/*Define the PAM configuration file as a constant*/
#define CONFIGFILE "/etc/pam_nfc.conf"

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

/*Function to get the number of NFC targets*/
int getTargets (char **targets[])
{
    /*Set the intial amount of targets to 0*/
    int numTargets = 0;

    /*Variable declarations*/
    *targets = malloc (32 * sizeof (char *));
    nfc_connstring devices[8];
    size_t deviceCount;
    size_t targetCount;
    size_t i;

    /*Start the NFC context and detect the number of devices*/
    nfc_context *context;
    nfc_init(&context);
    deviceCount = nfc_list_devices(context, devices, 8);

    /*Define the NFC modulation values*/
    nfc_modulation nm = {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_UNDEFINED
    };

    /*Loop through each NFC reader*/
    for (i = 0; i < deviceCount; i++) {
        /*Open a connection to the device*/
        nfc_device* initiator = nfc_open (context, devices[i]);
        
        if (initiator) {
            /*Initiate the connection*/
            nfc_initiator_init (initiator);
            nfc_target ant[32];
            int res;

            /*If there is passive targets*/
            if ((res = nfc_initiator_list_passive_targets (initiator, nm, ant, 32)) >= 0) {
                size_t iTarget;
                targetCount = res;

                /*Loop through the passive targets*/
                for (iTarget = 0; iTarget < targetCount; iTarget++) {
                    /*Allocate memory to the targets*/
                    if (((*targets)[numTargets] = malloc (2 * ant[iTarget].nti.nai.szUidLen + 1))) {
                        size_t n;
                        (*targets)[numTargets][0] = '\0';

                        for (n = 0; n < ant[iTarget].nti.nai.szUidLen; n++) {
                            sprintf ((*targets)[numTargets] + 2 * n, "%02x", ant[iTarget].nti.nai.abtUid[n]);
                        }
                        /*Increment the target calculator*/
                        numTargets++;
                    }
                }
            }
            /*Close the connection to the device*/
            nfc_close (initiator);
        }
    }
    /*Exit from libnfc*/
    nfc_exit(context);
    /*Return the number of targets detected*/
    return numTargets;
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