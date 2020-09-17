/*
 ============================================================================
 Name        : fps_led_glow.c
 Author      : Srikanth Pendem
 Created on	 : Jul. 13, 2020
 Copyright   : Your copyright notice
 Description : FPS in C, Ansi-style
 ============================================================================
 This code works for Beaglebone Black Rev3.
 Free to use and modify but please do not forget to recontribute.
 This code controls any of the 2 UARTS.
 ============================================================================
 The whole source code is written in C only.
 This source code will enroll a fingerprint.
 This source code will verify the fingerprint user has already created.
 This source code will also edit HTML and upload to the site.
 GT-521F32 Fingerprint Sensor
 ============================================================================
 */

#include<stdio.h>
#include<errno.h>
#include<stddef.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library
#include <string.h>
#include<time.h>

#include "GPS.h"
/*COMMADS TO CONTROL THE SENSOR*/
#define OPEN 		"\x55\xAA\x01\x00\x00\x00\x00\x00\x01\x00\x01\x01"
#define CLOSE		"\x55\xAA\x00\x00\x00\x00\x00\x00\x02\x00\x02\x01"
#define LED_ON		"\x55\xAA\x01\x00\x01\x00\x00\x00\x12\x00\x13\x01"
#define LED_OFF		"\x55\xAA\x01\x00\x00\x00\x00\x00\x12\x00\x12\x01"
#define IS_PRESS 	"\x55\xAA\x01\x00\x00\x00\x00\x00\x26\x00\x26\x01"
#define DELETE_ALL 	"\x55\xAA\x01\x00\x00\x00\x00\x00\x41\x00\x41\x01"
#define IDENTIFY	"\x55\xAA\x01\x00\x00\x00\x00\x00\x51\x00\x51\x01"
#define CAPTURE		"\x55\xAA\x01\x00\x00\x00\x00\x00\x60\x00\x60\x01"
#define ENROLL_ST	"\x55\xAA\x01\x00\x01\x00\x00\x00\x22\x00\x23\x01"
#define ENROLL_1	"\x55\xAA\x01\x00\x00\x00\x00\x00\x23\x00\x23\x01"
#define ENROLL_2	"\x55\xAA\x01\x00\x00\x00\x00\x00\x24\x00\x24\x01"
#define ENROLL_3	"\x55\xAA\x01\x00\x00\x00\x00\x00\x25\x00\x25\x01"
#define ENROLL_COUNT "\x55\xAA\x01\x00\x00\x00\x00\x00\x20\x00\x20\x01"
#define ACK 30

int mainfd, s;
unsigned char F_recieve[50];
char LED_OFF_ACK[50];
char LED_ON_ACK[50];
char IS_PRESS_ACK[50];
char DELETE_ALL_ACK[50];
char IDENTIFY_ACK[50];
char OPEN_ACK[50];
char CLOSE_ACK[50];
char CAPTURE_ACK[50];
char ENROLL_COUNT_ACK[50];
char cur_time[128];

char ENROLL_ST_ACK[50];
char ENROLL_1_ACK[50];
char ENROLL_2_ACK[50];
char ENROLL_3_ACK[50];
void open_port();
void webpage();
void webpage2();
int enroll_start()
    {
    write(mainfd, ENROLL_ST, 50);  //sends the command to start  enrollment
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); //reads the acknowledgment
    sprintf(ENROLL_ST_ACK, "%02X", F_recieve[8]);
    return atoi(ENROLL_ST_ACK);		// returns the acknowledgment
    }

int trial_1()
    {
    write(mainfd, ENROLL_1, 50);  //first trial
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48);  //reads the acknowledgment
    sprintf(ENROLL_1_ACK, "%02X", F_recieve[8]);
    return atoi(ENROLL_1_ACK);			// returns the acknowledgment
    }

int trial_2()
    {
    write(mainfd, ENROLL_2, 50);  			//second trial
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); //reads the acknowledgment
    sprintf(ENROLL_2_ACK, "%02X", F_recieve[8]);
    return atoi(ENROLL_2_ACK);			// returns the acknowledgment
    }

int finalize()
    {
    write(mainfd, ENROLL_3, 50);  			//final trial
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); 	//reads the acknowledgment
    sprintf(ENROLL_3_ACK, "%02X", F_recieve[8]);
    return atoi(ENROLL_3_ACK);			// returns the acknowledgment
    }

int finopen()
    {
    write(mainfd, OPEN, 50);  			//command to open the sensor
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48);  	//reads the acknowledgment
    sprintf(OPEN_ACK, "%02X ", F_recieve[8]);
    return atoi(OPEN_ACK);			// returns the acknowledgment
    }

int finclose()
    {
    write(mainfd, CLOSE, 50); 			//command to open the sensor
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); 	//reads the acknowledgment
    sprintf(CLOSE_ACK, "%02X", F_recieve[8]);
    return atoi(CLOSE_ACK);			//returns the acknowledgment
    }

int delete()
    {
    write(mainfd, DELETE_ALL, 50); //command to delete all the stored fingerprints
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); //reads the acknowledgment
    sprintf(DELETE_ALL_ACK, "%02X",
	    F_recieve[0] + F_recieve[1] + F_recieve[2] + F_recieve[3]
		    + F_recieve[4] + F_recieve[5] + F_recieve[6] + F_recieve[7]
		    + F_recieve[8] + F_recieve[9] + F_recieve[10]
		    + F_recieve[11]); //print the reception
    return atoi(DELETE_ALL_ACK);		//returns the acknowledgment
    }

int identify()
    {
    write(mainfd, IDENTIFY, 50); //command to identify the finger with database
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48);	//reads the acknowledgment
    sprintf(IDENTIFY_ACK, "%02X ", F_recieve[4]);
    return atoi(IDENTIFY_ACK);			//returns the acknowledgment
    }

int led_on()
    {
    write(mainfd, LED_ON, 50); 			//command to ON the LED
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); //reads the acknowledgment
    sprintf(LED_ON_ACK, "%02X", F_recieve[8]); //print the reception
    return atoi(LED_ON_ACK);			//returns the acknowledgment
    }

int led_off()
    {
    write(mainfd, LED_OFF, 50); 		//command to OFF the LED
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); 	//reads the acknowledgment
    sprintf(LED_OFF_ACK, "%02X ", F_recieve[8]); //print the reception
    return atoi(LED_OFF_ACK);			//returns the acknowledgment
    }

int capture()
    {
    //printf("Please place your finger.\n");
    write(mainfd, CAPTURE, 50);  		//command to capture the finger
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); //read the acknowledgment
    sprintf(CAPTURE_ACK, "%02X", F_recieve[8]);
    return atoi(CAPTURE_ACK);			//returns the acknowledgment
    }

int ispress()
    {
    write(mainfd, IS_PRESS, 50); 	//command to check if finger pressed
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); 	//read the acknowledgment
    sprintf(IS_PRESS_ACK, "%02X", F_recieve[10]);
    return atoi(IS_PRESS_ACK);				//returns the checksum
    }

int enroll_count()
    {
    write(mainfd, ENROLL_COUNT, 50); 	//command to count enrolled fingers
    sleep(1);
    read(mainfd, (void*) &F_recieve, 48); //read the acknowledgment
    sprintf(ENROLL_COUNT_ACK, "%02X ", F_recieve[4]);
    return atoi(ENROLL_COUNT_ACK);		//returns the acknowledgment
    }

/*This function verifies the available fingers stored in database*/
void verify()
    {
    int l = led_on();		//ON the LED
    if (l != ACK)
	{			//ON ERROR
	fprintf(stderr, "Cannot off the LED %s\n", strerror(errno));
	}
    sleep(3);
    int c = capture();		//capture the finger
    /*
     if (c != ACK) {
     printf("Try Again\n");	//try again if capture fails
     sleep(2);
     c = capture();
     if (c != ACK) {
     printf("Try Again\n");	//try again if capture fails
     sleep(2);
     c = capture();
     if (c != ACK) {
     fprintf(stderr, "Cannot capture ERR: %d\n", c);	//exit with an error if capture failed
     led_off();
     printf("Biometric Access is failed, please try alternatives\n");
     }
     }
     }
     */
    int a = 0;
    while (c != ACK)
	{
	sleep(2);
	c = capture();
	a++;
	}

    int i = identify();				//identify the finger captured
    if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4)
	{				//numbers are enrolled IDs
	/*printf("IDENTIFY_ACK: %d\n", i);*/
	printf("ID: %d :: ACCESS OK\n", enroll_count());
	FILE *file = fopen("/home/debian/site/fpskey", "r");//opens the key file
	if (!file)
	    {
	    printf("creating a key please wait....\n\n");
	    system("echo out > /home/debian/site/fpskey");
	    sleep(2);
	    printf(
		    "Please wait while your access is uploaded to the site...\n\n");
	    //webpage();
	    GPS();
	    led_off();
	    exit(0);
	    }
	else
	    {
	    printf(
		    "Please wait while your access is uploaded to the site...\n\n");
	    //webpage();
	    GPS();
	    led_off();
	    exit(0);
	    }

	}
    else if (i > 4)
	{
	fprintf(stderr, "Verification Failed!\n ");//Exits with error if finger is wrong.
	led_off();
	//webpage2();
	exit(1);
	}
    int lo = led_off();		//OFF the LED
    if (lo != ACK)
	{
	fprintf(stderr, "Cannot off the LED %s\n", strerror(errno));
	}
    }

/*This function enrolls the new fingerprint*/

int enroll()
    {
    printf("No biometrics are found\n");
    printf("Lets enroll your finger, You must place your finger 3 times\n");
    int c;
    int l = led_on();			//ON LED
    if (l != ACK)
	{
	fprintf(stderr, "Cannot lit the LED  ERR: %d\n", l);
	finclose();
	exit(0);
	}
    enroll_start();			//start enrollment process
    sleep(1);
    printf("Place your finger\n");
    sleep(3);

    c = capture();		//capture the finger
    if (c != ACK)
	{
	printf("Try Again\n");		//try again if capture fails
	sleep(2);
	c = capture();
	if (c != ACK)
	    {
	    printf("Try Again\n");		//try again if capture fails
	    sleep(2);
	    c = capture();
	    if (c != ACK)
		{
		fprintf(stderr, "Cannot capture ERR: %d\n", c);	//exit with an error if capture failed
		led_off();
		exit(1);
		}
	    }
	}

    sleep(2);
    printf("Remove your finger\n");
    sleep(2);
    trial_1();				//Stores the captured finger in buffer1
    printf("Place your finger\n");
    sleep(1);

    c = capture();			//capture the finger
    if (c != ACK)
	{
	printf("Try Again\n");		//try again if capture fails
	sleep(2);
	c = capture();
	if (c != ACK)
	    {
	    printf("Try Again\n");		//try again if capture fails
	    sleep(2);
	    c = capture();
	    if (c != ACK)
		{
		fprintf(stderr, "Cannot capture ERR: %d\n", c);	//exit with an error if capture failed
		led_off();			//OFF LED
		exit(1);
		}
	    }
	}
    sleep(2);
    printf("Remove your finger\n");
    sleep(2);
    trial_2();				//Stores the captured finger in buffer2
    printf("Place your finger\n");
    sleep(1);

    c = capture();				//capture the finger
    if (c != ACK)
	{
	printf("Try Again\n");		//try again if capture fails
	sleep(2);
	c = capture();
	if (c != ACK)
	    {
	    printf("Try Again\n");	//try again if capture fails
	    sleep(2);
	    c = capture();
	    if (c != ACK)
		{
		fprintf(stderr, "Cannot capture ERR: %d\n", c);	//exit with an error if capture failed
		led_off();	//OFF LED
		exit(1);
		}
	    }
	}
    sleep(2);
    printf("Remove your finger\n");
    sleep(2);
    finalize();	//compares 2 captures fingers then stores the final fingerprint, exits,

    int lo = led_off();		//LED OFF
    if (lo != ACK)
	{
	fprintf(stderr, "Cannot off the LED %s\n", strerror(errno));
	}
    return 0;
    }

int biometrics()
    {
    open_port();		//Opens the UART

    int o = finopen();			//opens the sensor
    if (o != ACK)
	{
	fprintf(stderr, "failed to open the Sensor ERR: %d\n", o);
	exit(1);
	}
    /*printf("%d",asd);*/
    if (enroll_count() != 0)	//if already finger registered
	verify();			//verifies the finger
    else
	{
	enroll();			//starts enrollment if no registration
	if (enroll_count() != 0)
	    printf("Registered successfully!\n");
	}
    if (enroll_count() != 0)
	{
	printf("Thanks!\n");
	exit(0);
	}
    else
	{
	printf("Failed to register\n");		//error in registration
	exit(1);
	}
    close(mainfd);		// closes the UART
    return 0;
    }

void open_port()
    {
    /* File descriptor for the port */
    system("config-pin P9.13 uart >/dev/null 2>&1");
    system("config-pin P9.11 uart >/dev/null 2>&1");
    mainfd = open("/dev/ttyO4", O_RDWR | O_NOCTTY /*| O_NDELAY*/);

    if (mainfd == -1)
	{ /* Could not open the port */
	fprintf(stderr, "open_port: Unable to open /dev/ttyO4 - %s\n",
		strerror(errno));
	}
    /* File descriptor */
    struct termios options;
    /* Get the current options for the port */
    tcgetattr(mainfd, &options);
    cfsetispeed(&options, B9600); /* Set the baud rates to 9600 */
    cfsetospeed(&options, B9600);

    /* Enable the receiver and set local mode */
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; /* Mask the character size to 8 bits, no parity */
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; /* Select 8 data bits */
    options.c_cflag &= ~CRTSCTS; /* Disable hardware flow control */

    /* Enable data to be processed as raw input */
    options.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcflush(mainfd, TCIFLUSH);        //discard file information not transmitted
    tcsetattr(mainfd, TCSANOW, &options);
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
    system("sed -i '$ d' /home/debian/site/fpsaccess.html >/dev/null 2>&1"); //deletes the last line
    FILE *webpage = fopen("/home/debian/site/fpsaccess.html", "a"); //opens the webpage
    if (!webpage)
	{
	//handle the error
	printf("something went wrong: %s", strerror(errno));
	exit(1);
	}
    /*---------------------Creates the HTML code---------------------*/
    char plink[100] = "<p>ID:1 Access Time and date: ";
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
	    "aws s3 cp /home/debian/site/fpsaccess.html s3://biometricbin2020/ --acl public-read >/dev/null 2>&1");//This command uploads the webpage to cloud
    printf(
	    "\n\nUpload Done\nPlease visit our website https://biometricbin2020.s3.ca-central-1.amazonaws.com/index.html for more info\n\n");
    }

void webpage2()
    {
    td();
    /*------------------------------------------------------*/
    /*EDIT THE webpage*/
    /*------------------------------------------------------*/
    system("sed -i '$ d' /home/debian/site/fpsfail.html >/dev/null 2>&1"); //deletes the last line
    FILE *webpage = fopen("/home/debian/site/fpsfail.html", "a"); //opens the webpage
    if (!webpage)
	{
	//handle the error
	printf("something went wrong: %s", strerror(errno));
	exit(1);
	}
    /*---------------------Creates the HTML code---------------------*/
    char plink[100] = "<p>ID: 1 Access was failed on: ";
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
	    "aws s3 cp /home/debian/site/fpsfail.html s3://biometricbin2020/ --acl public-read >/dev/null 2>&1");//This command uploads the webpage to cloud
    }
