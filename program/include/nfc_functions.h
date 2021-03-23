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