/*
 * Copyright (C) 2021 Thomas Read
 *
 * This program is free software; you
 * can redistribute it and/or modify
 * it as you wish. Please credit the project
 * owner in any papers or derived work where
 * this has been used.
 *
 * This project is now open source, and relies
 * on contributions from the community for
 * further development. Please see the README
 * for currently required features
 *
 */

#ifndef APDU_FUNCTIONS
#define APDU_FUNCTIONS

#include <nfc/nfc.h>

/*Get the user ID from the mobile device*/
char* getID();

/*Transmit to the card and get the response back*/
char* apduTransmit(nfc_device *pnd, uint8_t * capdu, size_t capdulen, uint8_t * rapdu, size_t * rapdulen);

#endif