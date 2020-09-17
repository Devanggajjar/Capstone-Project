/*
 * unlock.c
 *
 *  Created on: Aug. 6, 2020
 *      Author: srikanthpendem
 */
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<unistd.h>

#define sdb1_key "sudo cryptsetup luksOpen /dev/sdb1 encrypt --key-file /key/keypass >/dev/null 2>&1"
#define sdc1_key "sudo cryptsetup luksOpen /dev/sdc1 encrypt --key-file /key/keypass >/dev/null 2>&1"
#define sdd1_key "sudo cryptsetup luksOpen /dev/sdd1 encrypt --key-file /key/keypass >/dev/null 2>&1"
#define sde1_key "sudo cryptsetup luksOpen /dev/sde1 encrypt --key-file /key/keypass >/dev/null 2>&1"
#define sdf1_key "sudo cryptsetup luksOpen /dev/sdf1 encrypt --key-file /key/keypass >/dev/null 2>&1"
#define sdg1_key "sudo cryptsetup luksOpen /dev/sdg1 encrypt --key-file /key/keypass >/dev/null 2>&1"
#define sdh1_key "sudo cryptsetup luksOpen /dev/sdh1 encrypt --key-file /key/keypass >/dev/null 2>&1"

#define sdb1_fps "sudo cryptsetup luksOpen /dev/sdb1 encrypt --key-file /key/fpskey >/dev/null 2>&1"
#define sdc1_fps "sudo cryptsetup luksOpen /dev/sdc1 encrypt --key-file /key/fpskey >/dev/null 2>&1"
#define sdd1_fps "sudo cryptsetup luksOpen /dev/sdd1 encrypt --key-file /key/fpskey >/dev/null 2>&1"
#define sde1_fps "sudo cryptsetup luksOpen /dev/sde1 encrypt --key-file /key/fpskey >/dev/null 2>&1"
#define sdf1_fps "sudo cryptsetup luksOpen /dev/sdf1 encrypt --key-file /key/fpskey >/dev/null 2>&1"
#define sdg1_fps "sudo cryptsetup luksOpen /dev/sdg1 encrypt --key-file /key/fpskey >/dev/null 2>&1"
#define sdh1_fps "sudo cryptsetup luksOpen /dev/sdh1 encrypt --key-file /key/fpskey >/dev/null 2>&1"

#define script "\
echo  \"\n\nLet's check if you've LUKS\";\n\
sleep 2;\n\
dpkg -s cryptsetup > /dev/null 2>&1;\n\
if [ $? -eq 1 ] \n\
then\n\
echo \"\n\nOh, You don't have it. Let's install\";\n\
sleep 2;\n\
echo \"\n\nInstalling LUKS please wait\";\n\
sudo apt install -y cryptsetup > /dev/null;\n\
echo \"Installed\"; \n\
else \n\
sleep 2; \n\
echo \"\nNice, You've already installed the LUKS\"; \n\
fi "

#define script2 "echo temppwd | sudo sshfs debian@192.168.6.2:/home/debian/site /key -o password_stdin"

int main()
    {
    system("echo y | sudo apt install sshpass > /dev/null 2>&1");
    system("sshpass -p temppwd ssh debian@192.168.6.2 ./final_merge");
    system(script);    //it will run the script inside the c code.
    printf("\nPlease give the credentials  before continuing.....\n");
    system("sudo ls >/dev/null 2>&1");
    printf("\nGetting the key please wait.....\n");
    system(script2);	//mounts key to local system
    sleep(2);
    printf("\nlittle more time please.....\n");
    //searches for key
    system(sdb1_key);system(sdb1_fps);
    system(sdc1_key);system(sdc1_fps);
    system(sdd1_key);system(sdd1_fps);
    system(sde1_key);system(sde1_fps);
    system(sdf1_key);system(sdf1_fps);
    system(sdg1_key);system(sdg1_fps);
    system(sdh1_key);system(sdh1_fps);
    sleep(2);
    printf("\nOpened the devices, Now mounting.\n");
    system(
	    "sudo mount /dev/mapper/encrypt ~/Desktop/pendrive/ >/dev/null 2>&1");	// mounts the pendrive
    system("sudo chown $USER ~/Desktop/pendrive/ >/dev/null 2>&1");
    sleep(3);
    printf("\n\nDone, now you can use your device on the Desktop\n\n\n");
    }
