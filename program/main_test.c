/*Headers to include*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nfc/nfc.h>
#include <sys/stat.h>

#include "main_test.h"

/*Define the PAM configuration file as a constant*/
#define CONFIGFILE "/etc/pam_nfc.conf"

//extern int nfcauth_is_authorized (const char *user, char *target);
extern int nfcauth_get_targets (char **targets[]);

int main (int argc, char *argv[])
{
    int granted = nfcauth_authorize(argv[1]);

    if (granted == 1) {
        printf("Authorized\n");
        exit(EXIT_SUCCESS);
    }

    printf("Unauthorized\n");
    exit(EXIT_FAILURE);
}

int nfcauth_authorize (const char *user)
{
    int grant_access = 0;

    char **targets;
    int i, target_count;

    target_count = getTargets(&targets);

    for (i = 0; i < target_count; i++) {
        if (nfcauth_is_authorized (user, targets[i])) {
            grant_access = 1;
            break;
        }
    }

    for (i = 0; i < target_count; i++) {
        free (targets[i]);
    }
    free (targets);

    return grant_access;
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

int nfcauth_is_authorized (const char *user, char *target)
{
    int found = 0;
    FILE *config;

    char needle[BUFSIZ];

    snprintf (needle, BUFSIZ, "%s %s\n", user, crypt(target, "RC"));

    if ((config = fopen(CONFIGFILE, "r"))) {
        char buffer[BUFSIZ];
        while (!found && fgets (buffer, BUFSIZ, config)) {
            if (strcmp (buffer, needle) == 0) {
                found = 1;
            }
        }
        fclose (config);
    }

    return found;
}