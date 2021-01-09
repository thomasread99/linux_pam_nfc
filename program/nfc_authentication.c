/*Include the general C header files*/
#include <stdio.h>

/*Include the PAM header files*/
#include <security/pam_appl.h>
#include <security/pam_misc.h>

/*Linux header file to get the username of the current user*/
#include "unistd.h"

/*Include the libnfc header files*/
#include <nfc/nfc.h>

/*Include the header file for this program*/
#include "nfc_authentication.h"

static struct pam_conv conv = {
    misc_conv,
    NULL
};

int main(int argc, char *argv[])
{       
    //pam_authentication();
    nfc_scan();
}

int pam_authentication() {
    pam_handle_t *pamh = NULL;
    int retval;   

    /*Get the current user*/
    const char *user = getlogin();
    printf("The current user is %s\n", user);

    /*Create the PAM context and initiate the transaction*/
    retval = pam_start(CONFIG, user, &conv, &pamh);

    if (retval == PAM_SUCCESS) {
        /*Authenticate the user*/
        retval = pam_authenticate(pamh, 0);
    }

    if (retval == PAM_SUCCESS) {
        /*Determine if their account is valid*/
        retval = pam_acct_mgmt(pamh, 0);
    }

    /*Authenticated or not*/
    if (retval == PAM_SUCCESS) {
        fprintf(stdout, "Authenticated\n");
    }
    else {
        fprintf(stdout, "Not Authenticated\n");
    }

    /*Terminate the PAM transaction*/
    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        fprintf(stderr, "check_user: failed to release authenticator\n");
        exit(1);
    }

    /*Indicate success*/
    return (retval == PAM_SUCCESS ? 0:1);
}

/*Helper function to display hex data*/
static void print_hex(const uint8_t *pbtData, const size_t szBytes)
{
    size_t szPos;

    for (szPos = 0; szPos < szBytes; szPos++) {
        printf("%02x ", pbtData[szPos]);
    }
    printf("\n");
}

int nfc_scan() {
    nfc_device *pnd;
    nfc_target nt;

    /*Allocate a pointer to nfc_context*/
    nfc_context *context;

    /*Initisalize libnfc and set the nfc_context*/
    nfc_init(&context);

    if (context == NULL) {
        printf("Unable to init libnfc (malloc)\n");
        exit(EXIT_FAILURE);
    }

    /*Display libnfc version*/
    const char *acLibnfcVersion = nfc_version();
    //(void)argc;
    printf("uses libnfc %s\n", /*argv[0],*/ acLibnfcVersion);

    /*Open first detected nfc device*/
    pnd = nfc_open(context, NULL);

    if (pnd == NULL) {
        printf("ERROR: %s\n", "Unable to open NFC device.");
        exit(EXIT_FAILURE);
    }

    /*Set opened nfc device to initiator mode*/
    if (nfc_initiator_init(pnd) < 0) {
        nfc_perror(pnd, "nfc_initiator_init");
        exit(EXIT_FAILURE);
    }

    printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));    
    
    /*Poll for ISO14443A (MIFARE) tag*/
    const nfc_modulation nmMifare = {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_106
    };

    if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
        printf("The following (NFC) ISO14443A tag was found:\n");
        printf("    ATQA (SENS_RES): ");
        print_hex(nt.nti.nai.abtAtqa, 2);
        printf("    UID (NFCID%c): ", (nt.nti.nai.abtUid[0] == 0x08 ? '3' : '1'));
        print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
        printf("    SAK (SEL_RES): ");
        print_hex(&nt.nti.nai.btSak, 1);
        if (nt.nti.nai.szAtsLen) {
            printf("    ATS (ATR): ");
            print_hex(nt.nti.nai.abtAts, nt.nti.nai.szAtsLen);
        }
    }    

    /*Close nfc device*/
    nfc_close(pnd);

    /*Release the context*/
    nfc_exit(context);
    exit(EXIT_SUCCESS);
}