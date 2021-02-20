#include <stdlib.h>
#include <string.h>
#include <nfc/nfc.h>

/*Transmit to the card and get the response back*/
char* apduTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen)
{
    /*Variable declarations*/
    int res, length = 0;
    size_t szPos;
    char *id;

    /*Return null if device doesn't send any bytes*/
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