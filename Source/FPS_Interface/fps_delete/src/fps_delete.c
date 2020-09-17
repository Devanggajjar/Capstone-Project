/*
 ============================================================================
 Name        : fps_delete.c
 Author      : Srikanth Pendem
 Version     :
 Copyright   : Your copyright notice
 Description : FPS delete, Ansi-style
 ============================================================================
 */

#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library
#include <string.h>

/*COMMADS TO CONTROL THE SENSOR*/
#define DELETE_ALL 	"\x55\xAA\x01\x00\x00\x00\x00\x00\x41\x00\x41\x01"
#define OPEN 		"\x55\xAA\x01\x00\x00\x00\x00\x00\x01\x00\x01\x01"
#define CLOSE		"\x55\xAA\x00\x00\x00\x00\x00\x00\x02\x00\x02\x01"
#define LED_ON		"\x55\xAA\x01\x00\x01\x00\x00\x00\x12\x00\x13\x01"
#define LED_OFF		"\x55\xAA\x01\x00\x00\x00\x00\x00\x12\x00\x12\x01"
#define DELETE_ALL 	"\x55\xAA\x01\x00\x00\x00\x00\x00\x41\x00\x41\x01"
#define IDENTIFY	"\x55\xAA\x01\x00\x00\x00\x00\x00\x51\x00\x51\x01"
#define CAPTURE		"\x55\xAA\x01\x00\x00\x00\x00\x00\x60\x00\x60\x01"
#define ENROLL_COUNT "\x55\xAA\x01\x00\x00\x00\x00\x00\x20\x00\x20\x01"
#define ACK 30

int mainfd, s;
unsigned char recieve[50];
unsigned char finger[50];
char LED_OFF_ACK[50];
char LED_ON_ACK[50];
char IS_PRESS_ACK[50];
char DELETE_ALL_ACK[50];
char IDENTIFY_ACK[50];
char OPEN_ACK[50];
char CLOSE_ACK[50];
char CAPTURE_ACK[50];
char ENROLL_COUNT_ACK[50];
void open_port();

int finopen() {
	write(mainfd, OPEN, 50);  			//command to open the sensor
	sleep(1);
	read(mainfd, (void*) &recieve, 48);  			//reads the acknowledgment
	sprintf(OPEN_ACK, "%02X ", recieve[8]);
	return atof(OPEN_ACK);					// returns the acknowledgment
}

int finclose() {
	write(mainfd, CLOSE, 50); 				//command to open the sensor
	sleep(1);
	read(mainfd, (void*) &recieve, 48); 	//reads the acknowledgment
	sprintf(CLOSE_ACK, "%02X", recieve[8]);
	return atof(CLOSE_ACK);					//returns the acknowledgment
}

int delete() {
	write(mainfd, DELETE_ALL, 50); //command to delete all the stored fingerprints
	sleep(1);
	read(mainfd, (void*) &recieve, 48); //reads the acknowledgment
	sprintf(DELETE_ALL_ACK, "%02X",
			recieve[0] + recieve[1] + recieve[2] + recieve[3] + recieve[4]
					+ recieve[5] + recieve[6] + recieve[7] + recieve[8]
					+ recieve[9] + recieve[10] + recieve[11]); //print the reception
	return atof(DELETE_ALL_ACK);		//returns the acknowledgment
}

int identify() {
	write(mainfd, IDENTIFY, 50); //command to identify the finger with database
	sleep(1);
	read(mainfd, (void*) &recieve, 48);		 //reads the acknowledgment
	sprintf(IDENTIFY_ACK, "%02X ", recieve[4]);
	return atof(IDENTIFY_ACK);				//returns the acknowledgment
}

int led_on() {
	write(mainfd, LED_ON, 50); 			//command to ON the LED
	sleep(1);
	read(mainfd, (void*) &recieve, 48); //reads the acknowledgment
	sprintf(LED_ON_ACK, "%02X", recieve[8]); //print the reception
	return atof(LED_ON_ACK);				//returns the acknowledgment
}

int led_off() {
	write(mainfd, LED_OFF, 50); 		//command to OFF the LED
	sleep(1);
	read(mainfd, (void*) &recieve, 48); 	//reads the acknowledgment
	sprintf(LED_OFF_ACK, "%02X ", recieve[8]); //print the reception
	return atof(LED_OFF_ACK);			//returns the acknowledgment
}

int capture() {
	//printf("Please place your finger.\n");
	write(mainfd, CAPTURE, 50);  			//command to capture the finger
	sleep(1);
	read(mainfd, (void*) &recieve, 48); //read the acknowledgment
	sprintf(CAPTURE_ACK, "%02X", recieve[8]);
	return atof(CAPTURE_ACK);			//returns the acknowledgment
}

int enroll_count() {
	write(mainfd, ENROLL_COUNT, 50); 		//command to count enrolled fingers
	sleep(1);
	read(mainfd, (void*) &recieve, 48); //read the acknowledgment
	sprintf(ENROLL_COUNT_ACK, "%02X ", recieve[4]);
	return atof(ENROLL_COUNT_ACK);			//returns the acknowledgment
}

/*This function verifies the available fingers stored in database*/
void verify() {
	printf("Please place your finger to authenticate\n");
	int l = led_on();		//ON the LED
	if (l != ACK) {			//ON ERROR
		fprintf(stderr, "Cannot off the LED %s\n", strerror(errno));
	}
	sleep(3);
	int c = capture();		//capture the finger
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
				exit(0);
			}
		}
	}

	int i = identify();				//identify the finger captured
	if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4) {//numbers are enrolled IDs
		delete();
	} else if (i > 4) {
		fprintf(stderr, "Verification Failed!\n ");	//Exits with error if finger is wrong.
		led_off();
		finclose();
		exit(0);
	}
	int lo = led_off();
	if (lo != ACK) {
		fprintf(stderr, "Cannot off the LED %s\n", strerror(errno));
	}
}

int main() {
	open_port();		//Opens the UART

	int o = finopen();			//opens the sensor
	if (o != ACK) {
		fprintf(stderr, "failed to open the Sensor ERR: %d\n", o);
		exit(0);
	}

	if (enroll_count() != 0) {	//if already finger registered
		verify();			//verifies the finger
		printf("Deleted!\n");
	} else
		printf("No biometrics are registered!\n"); //if no biometrics registered
	return 0;
}

void open_port() {
	system("config-pin P9.13 uart >/dev/null 2>&1");
	system("config-pin P9.11 uart >/dev/null 2>&1");
	/* File descriptor for the port */
	mainfd = open("/dev/ttyO4", O_RDWR | O_NOCTTY /*| O_NDELAY*/);

	if (mainfd == -1) { /* Could not open the port */
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
