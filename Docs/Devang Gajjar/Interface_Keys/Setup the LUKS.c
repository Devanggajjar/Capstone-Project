/*
 * setup.c
 *
 *  Created on: Aug. 7, 2020
 *      Author: srikanthpendem and Devang Gajjar
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define script "\
echo -e \"\n\nLet's check if you've LUKS\";\n\
 sleep 2;\n\
dpkg -s cryptsetup > /dev/null 2>&1;\n\
if [ $? -eq 1 ] \n\
then\n\
echo -e \"\n\nOh, You don't have it. Let's install\";\n\
sleep 2;\n\
echo -e \"\n\nInstalling LUKS please wait\";\n\
sudo apt install -y cryptsetup > /dev/null;\n\
echo \"Installed\"; \n\
else \n\
sleep 2; \n\
echo \"Nice, You've already installed the LUKS\"; \n\
fi "

int main()
    {
    system(script);    //it will run the script inside the c code.
    char p[50] = "sudo cryptsetup luksFormat /dev/";
    char p1[50] = "sudo cryptsetup luksOpen /dev/";
    char q[50] = "--key-file /key/keypass";
    char s[50];
    char buf[500];
    char buf1[500];
    //system("lsblk");
    scanf("%s", s);
    sprintf(buf, "%s%s %s", p, s, q);// p = sudo cryptsetup luksFormat /dev/XXX --key-file /key/keypass;
    sprintf(buf1, "%s%s %s %s", p1, s, "encrypt", q);//sudo cryptsetup luksFormat /dev/XXX --key-file /key/keypass;

    printf("%s\n", buf);
    printf("%s", buf1);
    printf("\nEncrypting your device\n");
    //system(buf);
    printf("\nIt takes a while...\n");
    // system(buf1);
    printf("\nHmmm A bit more...\n");
    //system("sudo mkfs.vfat /dev/mapper/encrypt > /dev/null 2>&1");
    // system("sudo cryptsetup luksClose /dev/mapper/encrypt");
    printf("\nThanks for your patience, its encrypted now!\n");
    printf("\nPlease remove the device!\n");
    return 0;
    }