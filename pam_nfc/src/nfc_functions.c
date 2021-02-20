/*Headers to include*/
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
    char **targets;
    int i, targetCount;

    /*Get the number of NFC targets*/
    targetCount = getTargets(&targets);

    /*For each target found, check to see if it is authorised*/
    for (i = 0; i < targetCount; i++) {
        if (checkAuth (user, targets[i])) {
            authorised = 1;
            break;
        }
    }

    /*Free the memory*/
    for (i = 0; i < targetCount; i++) {
        free (targets[i]);
    }
    free (targets);

    /*Return the result*/
    return authorised;
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

    if (context = NULL) {
        printf("Unable to init libnfc (malloc)\n");
        exit(EXIT_FAILURE);
    }

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

/*Function to check if the user and tag are found in the config*/
int checkAuth (const char *user, char *target)
{
    /*Initially set to not found*/
    int found = 0;

    /*Variable declarations*/
    FILE *config;
    char needle[BUFSIZ];

    /*Write to the buffer*/
    snprintf (needle, BUFSIZ, "%s %s\n", user, crypt(target, "RC"));

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