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

#ifndef AUTHENTICATE
#define AUTHENTICATE

/*Define the PAM configration to use*/
#define CONFIG "login"

/*Function to authenticate through PAM*/
int pamAuthenticate (int argc, const char **argv);

#endif