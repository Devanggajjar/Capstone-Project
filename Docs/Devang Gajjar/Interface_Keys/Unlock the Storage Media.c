// THIS CODE UNLOCKS THE STORAGE MEDIA.

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
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

int main() {
    system(script);    //it will run the script inside the c code.
    printf("Please give the credentials  before continuing.....\n");
    system("sudo ls >/dev/null 2>&1");
    printf("Getting the key please wait.....\n");
    system("sudo sshfs debian@192.168.6.2:/home/debian/site /key");
    sleep(2);
    printf("little more time please.....\n");
    system("sudo cryptsetup luksOpen /dev/sdb1 encrypt --key-file /key/keypass >/dev/null 2>&1");
    system("sudo cryptsetup luksOpen /dev/sdc1 encrypt --key-file /key/keypass >/dev/null 2>&1");
    system("sudo cryptsetup luksOpen /dev/sdd1 encrypt --key-file /key/keypass >/dev/null 2>&1");
    system("sudo cryptsetup luksOpen /dev/sde1 encrypt --key-file /key/keypass >/dev/null 2>&1");
    system("sudo cryptsetup luksOpen /dev/sdf1 encrypt --key-file /key/keypass >/dev/null 2>&1");
    sleep(2);
    printf("Opened the devices, Now mounting.\n");
    system("sudo mount /dev/mapper/encrypt ~/Desktop/pendrive/ >/dev/null 2>&1");
    system("sudo chown $USER ~/Desktop/pendrive/ >/dev/null 2>&1");
    sleep(3);
    printf("Done, now you can use your device on the Desktop\n");
}
