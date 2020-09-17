/*
 * uartread.c
 *
 *  Created on: Jul. 13, 2020
 *      Author: srikanthpendem
 */
/*
-----------------------------------------------------------------
This code searches a string and then prints the whole string line.
------------------------------------------------------------------
*/


#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int main(){
   int file;
   file = open("/dev/ttyO4", O_RDWR | O_NOCTTY);

   	  struct termios options;               //The termios structure is vital
      tcgetattr(file, &options);            //Sets the parameters associated with file
      options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
      options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
      tcflush(file, TCIFLUSH);             //discard file information not transmitted
      tcsetattr(file, TCSANOW, &options);  //changes occur immmediately

       char recieve[256];  			//the string to receive
while(1) {
	int characters = read(file, (void*) &recieve, 256);       //reception of the string
	usleep(500);
	recieve[characters] = 0;

	if (strstr(recieve, "$GPRMC"))		//searches $GPRMC
		printf("%s\n",recieve);		//prints the whole line

    fflush(stdout);

}
	close(file);
       return 0;
}
