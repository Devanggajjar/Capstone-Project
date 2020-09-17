/*
 * encrypt.c
 *
 *  Created on: Aug. 16, 2020
 *      Author: srikanthpendem
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>
#define script "\
echo  \"\nLet's check if you've LUKS....\";\n\
sleep 2;\n\
dpkg -s cryptsetup > /dev/null 2>&1;\n\
if [ $? -eq 1 ] \n\
then\n\
echo  \"\n\nOh, You don't have it. Let's install\";\n\
sleep 2;\n\
echo \"\n\nInstalling LUKS please wait\";\n\
sudo apt install -y cryptsetup > /dev/null 2>&1;\n\
echo \"Installed\"; \n\
else \n\
sleep 2; \n\
echo \"Nice, You already have the LUKS installed\"; \n\
fi "
#define script2 "echo temppwd | sudo sshfs debian@192.168.6.2:/home/debian/site /key -o password_stdin"
int main()
{
    system(script);    //it will run the script inside the c code.
    printf("\nPlease give the credentials  before continuing.....\n");
    system(script2);
    printf("Getting the key please wait.....\n");

    char p[50] = "sudo cryptsetup luksFormat /dev/";
    char p1[50] = "sudo cryptsetup luksOpen /dev/";
    char p2[100] = "sudo cat /key/keypass | sudo cryptsetup luksAddKey --key-file=- /dev/";
    char q[50] = "--key-file /key/keypass";
    char q1[50] = "/key/fpskey";
    char q2[50] = "sudo umount -l /media/$USER/";
    char s[50]; char s1[50];
    char buf[500];
    char buf1[500];
    char buf2[500];
    char buf3[500];
    sleep(2);
    printf("Please type your device name: ");sleep(1.5);
    scanf("%s", s1);
    sprintf(buf3, "%s%s", q2,s1);
    system(buf3);
    printf("\nNow you'll see a list of available media devices. \n"); sleep(2);
    printf("Please type the device name you want to encrypt\n");sleep(1.5);
    printf("Usually named as sdb sdb1 sdc sdc1......\n\n");
    sleep(10);
    system("lsblk");
    sleep(3);
    printf("\nEnter the device name: ");
    scanf("%s", s);


    sprintf(buf, "%s%s %s", p, s, q);// p = sudo cryptsetup luksFormat /dev/XXX --key-file /key/keypass;
    sprintf(buf1, "%s%s %s %s", p1, s, "encrypt", q);//sudo cryptsetup luksFormat /dev/XXX --key-file /key/keypass;
    sprintf(buf2, "%s%s %s", p2, s, q1);//sudo cat /key/keypass | sudo cryptsetup luksAddKey --key-file=- /dev/sdb1 /key/fpskey;

    //printf("%s\n", buf);
    //printf("%s", buf1);
    printf("\nEncrypting your device\n");
    sleep(3);
    system(buf);
    printf("\nIt takes a while...\n");
    sleep(3);
    system(buf1);
    printf("\nHmmm A bit more...\n");
    system("sudo mkfs.ext4 /dev/mapper/encrypt > /dev/null 2>&1");	//
    sleep(3);
    printf("\nAdding new key please wait...\n");
    system(buf2);
    sleep(2);
    system("sudo cryptsetup luksClose /dev/mapper/encrypt");
    printf("\nThanks for your patience, its encrypted now!\n");
    printf("\nPlease remove the device!\n");
    return 0;
    }
