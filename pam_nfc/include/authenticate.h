#ifndef AUTHENTICATE
#define AUTHENTICATE

/*Define the PAM configration to use*/
#define CONFIG "login"

/*Function to authenticate through PAM*/
int pamAuthenticate (int argc, const char **argv);

#endif