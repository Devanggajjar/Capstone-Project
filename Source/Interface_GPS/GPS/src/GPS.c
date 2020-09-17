/*
 ============================================================================
 Name        : GPS.c
 Author      : Srikanth Pendem

This is GPS C code.
This code filters the GPRMC string.
This code prints Latitude, Longitude, Date and Time only.
This code also edits the HTML file
This code uploads the HTML file to the cloud.

This code is free to use and modify.
 ============================================================================
 */

#include<errno.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int file;				// file opener
char recieve[256];  	//reception buffer
char *p;				//this reads and filters the reception.
char *l;				//this reads and filters the reception.
double lat;				//latitude decimal coordinates
double lon;				//longitude decimal coordinates
char dir_lat;			//Latitude Cardinal Direction
char dir_lon;			//Longitude Cardinal Direction
double tm;				//gets the time
double dt;				//gets the date
int hr, min, sec;		//displays the time
int dd, mm, yr;			//displays the date
int c = 0;


/*Converts Degrees to Decimal Degrees*/
double deg_conv(double coord) {
	double deg, min, dec, lat;
	dec = coord / 100;
	deg = truncf(dec);   			// Gets the degrees
	min = ((dec - deg) * 100) / 60;	// Gets the decimal.
	lat = deg + min;
	return lat;						//displays the decimal location coordinates
}


/*Parses the time*/
void time(double tm) {
	double a, b, c;
	a = tm / 10000;
	hr = truncf(a);             // Get the hour

	b = (a - hr) * 100;
	min = truncf(b);            // Get the minutes

	c = (b - min) * 100;
	sec = truncf(c);			// Get the seconds
}

/*Parses the date*/
void date(double tm)	//160720
{
	double a, b, c;
	a = tm / 10000;
	dd = truncf(a);             // Get the day

	b = (a - dd) * 100;
	mm = truncf(b);             // Get the month

	c = (b - mm) * 100;
	yr = 2000 + truncf(c);		// Get the year
}

void UART() {
	system("config-pin P9.11 uart >/dev/null 2>&1");
	file = open("/dev/ttyO4", O_RDWR | O_NOCTTY);

	struct termios options;               //The termios structure is vital
	tcgetattr(file, &options);        //Sets the parameters associated with file
	options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
	options.c_iflag = IGNPAR;    //ignore partity errors, CR -> newline
	tcflush(file, TCIFLUSH);          //discard file information not transmitted
	tcsetattr(file, TCSANOW, &options);  //changes occur immmediately
}

int main() {
	UART();

	while (c != 10) {
		do {

			int characters = read(file, (void*) &recieve, 256); //reception of the string
			recieve[characters] = 0;

			l = strstr(recieve, "$GPRMC");		//Filters the GPRMC
			p = strstr(recieve, "$GPRMC");	//Filters the GPRMC

			usleep(500);

			if (p != NULL) {
				p = strchr(p, ',') + 1;			//Skips GPRMC
				//printf("%s", p);
			}

			if (p != NULL) {
				tm = atof(p);				//String to double.
				time(tm);					//Time conversion
				//printf("Time %d/%d/%d\n", hr, min, sec);
				p = strchr(p, ',') + 1;			//Skips Time
				//printf(" %s", p);
			}

			if (p != NULL) {
				p = strchr(p, ',') + 1;			//Skips Status
				//printf(" %s", p);
			}

			if (p != NULL) {
				lat = deg_conv(atof(p));//converts string to double and then to decimal coordinates
				//printf(" %lf", lat);			// Prints latitude
				p = strchr(p, ',') + 1;			//Skips Latitude
				//printf(" %s", p);
			}

			if (p != NULL) {
				switch (p[0])
				//checks the first array.
				{
				case 'N':
					dir_lat = 'N';			// If N, then puts the N
					//printf("%c ", lat);
					break;
				case 'S':
					dir_lat = 'S';			// If S, then puts the S
					//printf("%c ", lat);
					break;
				}
				p = strchr(p, ',') + 1;		//Skips Coordinate Direction
				//printf(" %c", dir_lat);		// Prints cardinal directions of Latitude
				//printf(" %s", p);
			}

			if (p != NULL) {
				lon = deg_conv(atof(p));//converts string to double and then to decimal coordinates
				//printf(" %lf", lon);			// Prints longitude
				p = strchr(p, ',') + 1;			//Skips Longitude
				//printf(" %s", p);
			}
			if (p != NULL) {
				{
					switch (p[0]) {
					case 'E':
						dir_lon = 'E';				// If E, then puts the E
						//printf("%c ", dir_lon);	//Prints cardinal directions of Longitude
						break;
					case 'W':
						dir_lon = 'W';				// If W, then puts the W
						//printf(" %c", dir_lon);		//Prints cardinal directions of Longitude
						break;
					}
					p = strchr(p, ',') + 1;			//Skips Coordinate Direction
					//printf(" %c", dir_lon);		//Prints cardinal directions of Latitude
					//printf(" %s", p);
				}							//lon switch ends
			}							//

			if (p != NULL) {
				p = strchr(p, ',') + 1;			//Skips Speed
				//printf(" %s", p);
			}

			if (p != NULL) {
				p = strchr(p, ',') + 1;			//Skips After Speed
				//printf(" %s", p);
			}

			if (p != NULL) {
				dt = atof(p);				//String to double.
				date(dt);					//Date conversion
				//printf("Date %d/%d/%d\n", dd, mm, yr);
				p = strchr(p, ',') + 1;			//Skips Date
				//printf(" %s", p);
			}

		}							//int while ends
		while (l != NULL);	// Prints time
		fflush(stdout);					//This is needed to work the delay.
		c++;
	}								//forever ends
	printf("Lat: %lf", lat);					//prints the latitude
	printf("\nLon: %lf", lon);					//prints the longitude
	printf("\nTime %02d:%02d:%02d\n", hr, min, sec);	//prints the time
	printf("Date %02d/%02d/%d\n", dd, mm, yr);		//prints the date
	close(file);
	sleep(3);
	/*------------------------------------------------------*/
	/*EDIT THE webpage*/
	/*------------------------------------------------------*/
	system("sed -i '$ d' /home/debian/site/locations.html >/dev/null 2>&1");//deletes the last line
	FILE *webpage = fopen("/home/debian/site/locations.html", "a");		//opens the webpage
	if (!webpage) {
		//handle the error
		printf("something went wrong: %s", strerror(errno));
		exit(1);
	}
	/*---------------------Creates the HTML code---------------------*/
	char buf[] = "<p>";
	char buffer[500];				//lat buffer string
	char buffer1[500];				//lon buffer string
	char buffer2[500];				//date buffer string
	char plink[100] = " <a href=\"https://www.google.com/maps/place/";

	sprintf(buffer2, "%s %d/%d/%d", buf, dd, mm, yr);//converts date to string
	//printf("\n%s\n",buffer2);
	strncat(buffer2, plink, 58);			//appends date to plink. (date_link)
	//printf("\n%s\n",buffer2);

	sprintf(buffer, "%lf", lat);				//converts latitude to string
	strncat(buffer, &dir_lat, 50);//appends cardinal direction to latitude string
	strncat(buffer, ",", 50);					// appends ","

	sprintf(buffer1, "%lf", lon);				//converts latitude to string
	strncat(buffer1, &dir_lon, 50);	//appends cardinal direction to longitude string

	strncat(buffer, buffer1, 200);//appends latitude string and longitude string (lat_lon string)
	//printf("\n%s\n",buffer);

	strncat(buffer2, buffer, 200);		//appends lat_lon string with date_link
	//printf("\n%s\n",buffer2);

	strncat(buffer2,
			"\" target=\"_blank\" class=\"button2\" >GPS LOCATION</a></p> ",
			200);	//This is the whole  location link

	fprintf(webpage, "\n%s\n", buffer2);			//appends the whole link to the webpage
	fprintf(webpage,"\n%s", "</div> <div class=\"footer\"> <p>Designed by Srikanth Pendem</p> </div></body> </html>"); //appends the footer and ends the HTML
	//printf("\n%s\n",buffer2);

	fclose(webpage);				//close the webpage
	/*------------------------------------------------------*/
	/*UPLOAD THE webpage*/
	/*------------------------------------------------------*/
	system("aws s3 cp /home/debian/site/locations.html s3://biometricbin2020/ --acl public-read >/dev/null 2>&1");				//This command uploads the webpage to cloud
	printf(
			"\n\nUpload Done\nPlease visit our website https://biometricbin2020.s3.ca-central-1.amazonaws.com/index.html for more info\n\n");
	return 0;
}
