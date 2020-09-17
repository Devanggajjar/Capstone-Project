/*
 ============================================================================
 Name        : keyaccess.c
 Author      : Srikanth Pendem
 Copyright   : Your copyright notice
 Created on  : Aug. 8, 2020
 Description : keyaccess in C, Ansi-style
 ============================================================================
This code works for Beaglebone Black Rev3.
Free to use and modify but please do not forget to recontribute.
This code controls the GPIO P9.12
This code controls the GPIO P9.14
============================================================================
The whole source code is written in C only.
This source code will create a binary password.
This source code will create a password as user desired.
This source code will verify the password user has already created.
This source code will also edit HTML and upload to the site.
============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define P9_23 "/sys/class/gpio/gpio49/value"
#define P9_41 "/sys/class/gpio/gpio20/value"

/****Global variables***/
int c = 0; /*Counter variable*/
int f = 0; /*Counter variable*/
int g = 0; /*Counter variable*/
int fd;	 /*file descriptor*/
char buf[1];	 /*stores GPIO value*/
char buff[50];
char cur_time[128];
char pss[50];	//stores the password created
char vss[50];	// stores the verified password
char tss[50];	// compares the stored password
char key[50];	//reads the verified password
char file[50];	//reads the verified password
int q = 0;		//control flag
void webpage();	// upload function decaration
/*Button Press Function*/
int b2()
    {
    fd = open(P9_41, O_RDONLY); /*Opens GPIO value file*/
    read(fd, buf, 1); /*Reads the GPIO's gpio20 value and stores it*/
    int a = atof(buf);
    close(fd); /*Closes the GPIO file*/
    return a; /*Function returns the stored GPIO value*/

    } /*Function ends*/
/*Button Press Function*/
int b1()
    {
     fd = open(P9_23, O_RDONLY); /*Opens GPIO value file*/
    read(fd, buf, 1); /*Reads the gpio49 value and stores it*/
    int a = atof(buf);
    close(fd); /*Closes the GPIO file*/
    return a; /*Function returns the stored GPIO value*/

    } /*Function ends*/

/*This function creates a password for the user.*/
void makepss()
    {
    printf("Please give a password: ");
    while (c != 7)		// Counts 7 button presses
	{
	sleep(1);
	if (b1() != 0)
	    {
	    c++;		//counts if b1 is pressed
	    strncat(pss, "b1", 3);	// adds "b1" to a string array
	    }
	if (b2() != 0)
	    {
	    c++;			//counts if b1 is pressed
	    strncat(pss, "b2", 3); // adds "b2" to a string array
	    }
	if (b1() != 0 || b2() != 0)
	    printf("* ");	// prints the button presses as *
	fflush(stdout); /*This is required to work the sleep()*/
	}
    }
/*This function verifies the password that was created.*/
void verifypss()
    {
    printf("\nVerify the  password: ");
    while (f != 7)
	{
	sleep(1);
	if (b1() != 0)
	    {
	    f++;			//counts if b1 is pressed
	    strncat(vss, "b1", 3);	// adds "b1" to a string array
	    }
	if (b2() != 0)
	    {
	    f++;		//counts if b2 is pressed
	    strncat(vss, "b2", 3);	// adds "b2" to a string array
	    }
	if (b1() != 0 || b2() != 0)
	    printf("* ");
	fflush(stdout); /*This is required to work the sleep()*/
	}
    }

/*This function verifies the password that is stored.*/
const char* enterpss()
    {
    printf("\nEnter password to access: ");
    while (g != 7)
	{
	sleep(1);
	if (b1() != 0)
	    {
	    g++;		//counts if b1 is pressed
	    strncat(tss, "b1", 3);	// adds "b1" to a string array
	    }
	if (b2() != 0)
	    {
	    g++;		//counts if b2 is pressed
	    strncat(tss, "b2", 3);	// adds "b2" to a string array
	    }
	if (b1() != 0 || b2() != 0)
	    printf("* ");
	fflush(stdout); /*This is required to work the sleep()*/
	}
    return vss;		//returns the entered password string
    }

int main()
    {
    FILE *key = fopen("/home/debian/site/key", "r");	//opens the key file
    if (!key)
	{					//if key is not available
	makepss();			//prompts user to create a password
	verifypss();		//asks user to verify the entered password
	key = fopen("/home/debian/site/key", "w");	//creates a new key file
	int r = strcmp(vss, pss);		//if verification is successful
	if (r == 0)
	    {
	    fprintf(key, "%s", vss);		//adds the key to the keyfile
	    }
	else
	    {
	    printf("\nVerification falied\n");	//error
	    q = 1;		 /*invokes the flag*/
	    }
	}
    else	 /*If Key file already exists*/
	{
	int re = open("/home/debian/site/key", O_RDONLY);//this reads the keyfile
	read(re, buff, 50);
	enterpss();
	int r = strcmp(tss, buff);//compares the key entered and key previously created
	if (r == 0)
	    {
	    printf("\n\nACCESS OK\n");
	    FILE *file = fopen("/home/debian/site/keypass", "r");//opens the key file
	    if (!file)
		{
		printf("creating a key please wait....\n\n");
		system("echo out > /home/debian/site/keypass");
		sleep(2);
		printf(
			"Please wait while your access is uploaded to the site...\n\n");
		//webpage();
		}
	    else
		{
		printf(
			"Please wait while your access is uploaded to the site...\n\n");
		//webpage();
		}

	    }
	else
	    printf("ACCESS DENIED");
	}
    if (q == 1)
	{
	sleep(2);
	system("rm /home/debian/site/key");
	}
    return 0;
    }

/*Date and Time Function*/
const char* td()
    {
    time_t t;
    struct tm *ptm;
    t = time(NULL);
    ptm = localtime(&t);
    strftime(cur_time, 128, "%d-%b-%Y %H:%M:%S", ptm);
    //printf("Current date and time: %s\n", cur_time);
    return cur_time;
    }

void webpage()
    {
    td();
    /*------------------------------------------------------*/
    /*EDIT THE webpage*/
    /*------------------------------------------------------*/
    system("sed -i '$ d' /home/debian/site/keyaccess.html >/dev/null 2>&1");//deletes the last line
    FILE *webpage = fopen("/home/debian/site/keyaccess.html", "a");//opens the webpage
    if (!webpage)
	{
	//handle the error
	printf("something went wrong: %s", strerror(errno));
	exit(1);
	}
    /*---------------------Creates the HTML code---------------------*/
    char plink[100] = "<p>Access Time and date: ";
    strncat(plink, cur_time, 50);
    strncat(plink, "</p>", 50);
    fprintf(webpage, "\n%s\n", plink);
    fprintf(webpage, "\n%s",
	    "</div> <div class=\"footer\"> <p>Designed by Srikanth Pendem</p> </div></body> </html>"); //appends the footer and ends the HTML
    //printf("\n%s\n",plink);
    fclose(webpage);				//close the webpage
    /*------------------------------------------------------*/
    /*UPLOAD THE webpage*/
    /*------------------------------------------------------*/
    system(
	    "aws s3 cp /home/debian/site/keyaccess.html s3://biometricbin2020/ --acl public-read >/dev/null 2>&1");//This command uploads the webpage to cloud
    printf(
	    "\n\nUpload Done\nPlease visit our website https://biometricbin2020.s3.ca-central-1.amazonaws.com/index.html for more info\n\n");
    }
