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

#ifndef NFC_FUNCTIONS
#define NFC_FUNCTIONS

/*Path to the config file*/
#define CONFIGFILE "/etc/pam_nfc.conf"

/*Function to check if the user is authorised with their tag*/
int authorise (const char *user);

/*Function to get the number of NFC targets*/
int getTargets (char **targets[]);

/*Function to check if the user and tag are found in the config*/
int checkAuth (const char *user, char *target);

/*Check that the config file exists and can be read*/
int checkFile (void);

#endif