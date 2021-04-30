#include <stdlib.h>
#include <string.h>
#include <nfc/nfc.h>

#include "../include/apdu_functions.h"

/*Get the user ID from the mobile device*/
char* getID()
{
    /*Variable declarations*/
    char *id;
    nfc_device *initiator;
    nfc_target nt;
    nfc_context *context;    
    uint8_t capdu[264];
    size_t capdulen;
    uint8_t rapdu[264];
    size_t rapdulen;

    /*Start the NFC context*/
    nfc_init(&context);
    if (context == NULL) {
        printf("Unable to init libnfc (malloc)\n");
        return NULL;
    }    

    /*Open a connection to the device*/
    initiator = nfc_open(context, NULL);

    /*Check the connection has been properly initiated*/
    if (initiator == NULL) {
        printf("ERROR: %s", "Unable to open NFC device.");
        return NULL;
    }
    if (nfc_initiator_init(initiator) < 0) {
        nfc_perror(initiator, "nfc_initiator_init");
        return NULL;
    }    

    /*Define the NFC modulation values*/
    const nfc_modulation nmMifare = {
        .nmt = NMT_ISO14443A,
        .nbr = NBR_106,
    };

    /*Wait for the mobile device to be placed on the reader*/
    printf("Waiting for device...\n");
    while (nfc_initiator_select_passive_target(initiator, nmMifare, NULL, 0, &nt) <= 0);
    printf("Device detected\n");
    
    /*AID of the application*/
    memcpy(capdu, "\x00\xA4\x04\x00\x07\xA0\x00\x00\x02\x47\x10\x01\x00", 13);
    capdulen = 13;
    rapdulen = sizeof(rapdu);
    /*Call the apdu transmit function*/
    id = apduTransmit(initiator, capdu, capdulen, rapdu, &rapdulen);
    if (id == NULL)
        return NULL;
    
    /*Close the device and the context*/
    nfc_close(initiator);
    nfc_exit(context);

    return id;
}

/*Transmit to the card and get the response back*/
char* apduTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen)
{
    /*Variable declarations*/
    int res, length = 0;;
    size_t szPos;
    char *id; 

    /*Return null if device doesn't send any data*/
    if ((res = nfc_initiator_transceive_bytes(pnd, capdu, capdulen, rapdu, *rapdulen, 500)) < 0) {
        return NULL;
    }
    else {
        *rapdulen = (size_t) res;   
        /*Append each byte received to a string*/     
        for (szPos = 0; szPos < *rapdulen; szPos++) {
            length += sprintf(id + length, "%02x", rapdu[szPos]);            
        }
        
        return id;
    }
}