#ifndef _NFC_AUTHENTICATION_H
#define _NFC_AUTHENTICATION_H

/*Define which PAM config file will be used*/
#define CONFIG "login"

int pam_authentication();
static void print_hex(const uint8_t *pbtData, const size_t szBytes);
int nfc_scan();

#endif