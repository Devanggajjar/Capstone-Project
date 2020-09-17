/*
 * safelyremove.c
 *
 *  Created on: Aug. 6, 2020
 *      Author: srikanthpendem and Devang Gajjar
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {
    printf("Unmounting...\n");
    system("sudo umount ~/Desktop/pendrive/ >/dev/null 2>&1");
    sleep(1);
    system("sudo cryptsetup luksClose /dev/mapper/encrypt >/dev/null 2>&1");
    printf("Please wait...\n");
    sleep(1);
    system("sudo rm /key/keypass >/dev/null 2>&1");
    system("sudo umount -l  /key >/dev/null 2>&1");
    printf("Safe to remove!\n");
}


