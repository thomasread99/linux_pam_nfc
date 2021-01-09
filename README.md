# linux_pam_nfc
## Description
Using Linux Pluggable Authentication Modules (PAM) to authenticate users via the use of Near-Field Communication (NFC)

## Compilation
To compile a PAM program using gcc on the Linux command line, run the command
<br />
`gcc -o program program.c -lpam -lpam_misc`
<br />
and run it with
<br />
`./program`
<br />
<br />
To compile NFC program using gcc on the Linux command line, run the command
<br />
`gcc -o program program.c -lnfc`
<br />
and run it with
<br />
`./program`
<br />
<br />
To compile a program that uses crypt, run the command
<br />
`gcc -o program program.c -lcrypt

## PAM Application setup
PAM applications must contain certain header files in order to work. These files can be found in <br />
`usr/include/security`
<br />
 If they do not yet exist, install them from the command line by using 
 <br />
 `sudo apt-get install libpam0g-dev`

## NFC Application setup
In order to use NFC in Linux with C support, you must use the free libnfc library. This can be installed using <br />
`sudo apt-get install libnfc-bin libnfc-examples libnfc-pn53x-examples`
<br />
in Debian or Ubuntu based systems.
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


## Useful Links
- [Linux-PAM Application Developers' Guide](http://www.linux-pam.org/Linux-PAM-html/Linux-PAM_ADG.html)
- [Configure and use Linux-PAM](https://likegeeks.com/linux-pam-easy-guide/)
- [How to Configure and use PAM in Linux](https://www.tecmint.com/configure-pam-in-centos-ubuntu-linux/)
- [libnfc GitHub page](https://github.com/nfc-tools/libnfc)
- [libnfc Wiki](http://nfc-tools.org/index.php/Main_Page)
- [Stack Overflow](https://stackoverflow.com/)
