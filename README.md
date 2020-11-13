# linux_pam_nfc
## Description
Using Linux Pluggable Authentication Modules (PAM) to authenticate users via the use of Near-Field Communication (NFC)

## Compilation
To compile a PAM program using gcc on the Linux command line, run the command
`gcc -o program program.c -lpam -lpam_misc`
and run it with
`./program`

## Headers to include
PAM applications must contain certain header files in order to work. These files can be found in `usr/include/security`. If they do not yet exist, install them from the command line by using `sudo apt-get install libpam0g-dev`

## Useful Links
- [Linux-PAM Application Developers' Guide](http://www.linux-pam.org/Linux-PAM-html/Linux-PAM_ADG.html)
- [Configure and use Linux-PAM](https://likegeeks.com/linux-pam-easy-guide/)
- [How to Configure and use PAM in Linux](https://www.tecmint.com/configure-pam-in-centos-ubuntu-linux/)
- [Stack Overflow](https://stackoverflow.com/)
