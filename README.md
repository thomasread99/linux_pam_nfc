# linux_pam_nfc
## CONTENTS
1. [Description](#DESCRIPTION)
2. [Contribution](#CONTRIBUTION)
3. [Acknowledgement](#ACKNOWLEDGEMENT)
4. [Prerequisites](#PREREQUISITES)
5. [Setup](#SETUP)
6. [Compilation](#COMPILATION)
7. [Usage](#USAGE)
8. [To do](#TODO)
9. [Useful Links](#USEFUL LINKS)
10. [Troubleshooting](#TROUBLESHOOTING)


## DESCRIPTION
This program uses Linux Pluggable Authentication Modules (PAM) to authenticate users via the use of Near-Field Communication (NFC). It can be used with traditional NFC smart cards, as long as they are configured correctly, although the intended use of this program is with the OS_auth_application Android App.

## CONTRIBUTION
If you would like to be added as a contributor to this project, please get in touch with the owner via email: thomas@readcode.co.uk

## ACKNOWLEDGEMENT
Whilst this project is novel work, it takes inspiration from [pam_nfc](https://github.com/nfc-tools/pam_nfc). Thank you to the developers of that project for releasing it under a GNU General Public license so that it could be used to benefit this work. 

## PREREQUISITES
In order to use this program, you must have:
* A Linux system (preferably Debian based)
* libnfc installed on your system (see setup for details)
* A physical NFC card reader and tags
* Create an empty file called pam_nfc.conf in /etc/

## SETUP
### NFC SETUP
In order to use NFC in Linux with C support, you must use the free libnfc library. This can be installed using <br />
`sudo apt-get install libnfc-bin libnfc-examples libnfc-pn53x-examples`
<br />
in Debian based systems.
<br />
If using an ACR122 reader, as in the project, a few extra steps must be taken in order for it to work properly. Firstly, pcscd must be installed using <br />
`sudo apt-get install pcscd`
<br />
and pcsc which can be installed from [Here](https://pcsclite.apdu.fr/files/) and by following the instructions in the README.
<br />
Following this, the `libccid_Info.plist` file located within `etc` must be modified. Locate `<key>ifdDriverOptions</key>` and turn `<string>0x0000</string>` value into `0x0001`.
<br />
Finally, run
<br />
`sudo vim /etc/modprobe.d/blacklist-libnfc.conf`
<br />
and add the line `blacklist pn533_usb`, before rebooting your machine. libnfc should now be configured correctly, which can be verified by running
<br />
`nfc-list`
<br />
<br />
To use libnfc within C, run on the command line
<br />
`sudo apt-get install libnfc-dev`
<br />
and then at the top of the C file `#include <nfc/nfc.h>`

### LINUX-PAM SETUP
PAM applications must contain certain header files in order to work. These files can be found in <br />
`usr/include/security`
<br />
If they do not yet exist, install them from the command line by using 
<br />
`sudo apt-get install libpam0g-dev`

## COMPILATION
For ease of compilation, a makefile has been included with this solution. To compile both programs, simply run:
<br />
`cd program`
<br />
`make`

## USAGE
Once the program has been compiled, two executable files will be generated in the `program` folder.
### add_user
The add_user program is used to add new users to the pam_nfc.conf file. To use this program, simply run:
<br />
`sudo ./add_user $USERNAME$`
<br />
where `$USERNAME` is replaced with the username of the user you are adding. The program will establish a connection to the NFC reader where it will wait for the device to be touched to the reader. Once the device has been touched to the reader, it will perform the APDU transaction, and assuming everything is setup correctly, add the new user and their authentication ID to the config file.

### authenticate
The authenticate program carrys out the actual user authentication with Linux-PAM, using the entries in the config file. To use this program, simply run:
<br />
`sudo ./authenticate`
<br />
This program takes in no parameters via the command line, but Linux-PAM will prompt you to enter your username once the program is running. Once again, a connection to the NFC reader will be established, and once a device is touched, the APDU transaction is performed. The authentication ID received will then be compared to the configuration file, and Linux-PAM handles the authentication as a result.

## TODO
### PRIORITY
- Get project to actually unlock the computer and not just run from the command line
- Use a secure channel for the NFC communication
- UI for adding and managing users

### EXTENSION
*Please get in touch with the owner of this project before attempting anything from this section - thomas@readcode.co.uk*
- Windows/MacOS version

## USEFUL LINKS
- [Linux-PAM Application Developers' Guide](http://www.linux-pam.org/Linux-PAM-html/Linux-PAM_ADG.html)
- [Configure and use Linux-PAM](https://likegeeks.com/linux-pam-easy-guide/)
- [How to Configure and use PAM in Linux](https://www.tecmint.com/configure-pam-in-centos-ubuntu-linux/)
- [libnfc GitHub page](https://github.com/nfc-tools/libnfc)
- [libnfc Wiki](http://nfc-tools.org/index.php/Main_Page)
- [Stack Overflow](https://stackoverflow.com/)

## TROUBLESHOOTING
If you have any problems with setting up, running, or using this program, please do not hesitate to get in touch.
**Owners contact email:** thomas@readcode.co.uk
