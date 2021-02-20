#ifndef APDU_FUNCTIONS
#define APDU_FUNCTIONS

#include <nfc/nfc.h>

/*Transmit to the card and get the response back*/
char* apduTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen);

#endif