#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nfc/nfc.h>

#include "add_user.h"

void usage (char *progname);

void usage (char *progname)
{
    fprintf (stderr, "usage: %s username\n", progname);
    exit(EXIT_FAILURE);
}

int main (int argc, char *argv[])
{
    if (argc != 2)
        usage (argv[0]);

    char **targets;
    int n;

    /*if (1 != (n = nfcauth_get_targets (&targets))) {
        errx (EXIT_FAILURE, "%d targets detected", n);
    }*/

    n = nfcauth_get_targets (&targets);

    printf("%d target(s) detected\n", n);

    free (targets[0]);
	free (targets);

    exit(EXIT_SUCCESS);
}

int nfcauth_get_targets (char **targets[])
{
    int ret = 0;
    *targets = malloc (32 * sizeof (char *));
    nfc_connstring devices[8];
    size_t device_count;
    size_t target_count;
    size_t i;

    nfc_context *context;
    nfc_init(&context);
    device_count = nfc_list_devices(context, devices, 8);

    nfc_modulation nm = {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_UNDEFINED
    };

    for (i = 0; i < device_count; i++) {
        nfc_device* initiator = nfc_open (context, devices[i]);
        if (initiator) {
            nfc_initiator_init (initiator);
            nfc_target ant[32];
            int res;
            if ((res = nfc_initiator_list_passive_targets (initiator, nm, ant, 32)) >= 0) {
                size_t iTarget;
                target_count = res;
                for (iTarget = 0; iTarget < target_count; iTarget++) {
                    if (((*targets)[ret] = malloc (2 * ant[iTarget].nti.nai.szUidLen + 1))) {
                        size_t n;
                        (*targets)[ret][0] = '\0';
                        for (n = 0; n < ant[iTarget].nti.nai.szUidLen; n++) {
                            sprintf ((*targets)[ret] + 2 * n, "%02x", ant[iTarget].nti.nai.abtUid[n]);
                        }
                        ret++;
                    }
                }
            }
            nfc_close (initiator);
        }
    }
    nfc_exit(context);
    return ret;
}