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

/*Headers to include*/
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

#include "../include/add_user.h"
#include "../include/nfc_functions.h"
#include "../include/apdu_functions.h"

int main (int argc, char *argv[])
{
    /*Check that the user has supplied their username as a parameter*/
    if (argc != 2)
    {
        fprintf(stderr, "Add your username as a parameter\n");
        exit(EXIT_FAILURE);
    }    

    /*Variable declarations*/
    char *id, *name;
    
    name = argv[1];
    /*Set id to the id received from the device*/
    id = getID();    

    /*Check that an ID has been received*/
    if (id == NULL) {
        errx (EXIT_FAILURE, "Error communicating with device\n");
    }    

    /*Run the function to add the user id to the configuration file and error check*/
    if (!(addAuth (name, id))) {
        err (EXIT_FAILURE, "Error adding authorization for the user\n");
    }       

    /*Exit and report success*/
    printf("User added\n");
    exit(EXIT_SUCCESS);
}

int addAuth (char *user, char *id) 
{
    /*Variable declarations*/
    int ret;
    FILE *config;

    /*If the config file exists, check that it is read-only*/
    if ((config = fopen (CONFIGFILE, "r"))) {
        fclose (config);
        /*Chmod the file so it can be written to*/
        chmod (CONFIGFILE, 0600);
    }

    /*If no file exists, avoid race condition*/
    umask (0077);

    /*If you can't append to the file, return*/
    if (!(config = fopen (CONFIGFILE, "a"))) {
        return 0;
    }

    /*Write to the config file*/
    ret = (fprintf (config, "%s %s\n", user, crypt(id, "RC")) > 0);

    /*If you can't close the config file, return*/
    if (fclose (config) != 0) {
        return 0;
    }

    /*Set the file back to read-only*/
    chmod (CONFIGFILE, 0400);

    /*Return*/
    return ret;
}