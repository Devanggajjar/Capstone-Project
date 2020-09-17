/*
 ============================================================================
 Name        : Counter.c
 Author      : Srikanth Pendem
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 This code controls the GPIO P9.12
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include<sys/types.h>
#include<sys/stat.h>

/****Global variables***/
int fd; 				/*open GPIO*/
char buf[1];			/*Stores GPIO Value*/

/*Button Press Function*/
char Ispressed()
{
	fd = open("/sys/class/gpio/gpio44/value", O_RDONLY);	/*Opens GPIO value file*/
	read(fd, buf, 1);			/*Reads the GPIO's value and stores it*/
	close(fd);					/*Closes the GPIO file*/
	return *buf;				/*Function returns the stored GPIO value*/

}								/*Function ends*/

/*Main Function*/
int main()
{
    while(1)					/*A forever running loop*/
  {
    if (Ispressed() == '1') {	/*If GPIO value has '1'*/
    	sleep(01);				/*Sleeps 1 second*/					/* Counts the instance increments*/
    	printf (" PRESSED ");		/*Prints the instance counted number*/
    	fflush(stdout);			/*This is required to work the sleep()*/
    		}					/*Conditional loop ends*/
    else if (Ispressed() == '0') {
		sleep(01);				/*Sleeps 1 second*/					/* Counts the instance increments*/
		printf (" NOT PRESSED ");	/*Prints the instance counted number*/
		fflush(stdout);			/*This is required to work the sleep()*/
    }							/*Conditional loop ends*/
    	}						/*forever loop ends*/

    }							/*Main ends*/
