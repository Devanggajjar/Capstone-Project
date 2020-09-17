/*
 ============================================================================
 Name        : Counter.c
 Author      : Devang Gajjar
 Version     :
 Copyright   : Your copyright notice
 Description : Key Access, Ansi-style
 ============================================================================
 */

/*
 ============================================================================
 This code works for Beaglebone Black Rev3.
 Free to use and modify but please do not forget to recontribute.
 This code controls the GPIO P8.12
  This code controls the GPIO P8.14
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include<errno.h>
#include<string.h>

/****Global variables***/
int c = 0; /*Counter variable*/
int d = 0; /*Counter variable*/
int i = 0;
int e[7]; /*Counter variable*/
int f = 0; /*Counter variable*/
int fd; /*open GPIO*/
char buf[1]; /*Stores GPIO Value*/
char cur_time[128];

/*Button Press Function*/
char b2()
    {
    fd = open("/sys/class/gpio/gpio26/value", O_RDONLY); /*Opens GPIO value file*/
    read(fd, buf, 1); /*Reads the GPIO's 14 PIN value and stores it*/
    int a = atof(buf);
    close(fd); /*Closes the GPIO file*/
    return a; /*Function returns the stored GPIO value*/

    } /*Function ends*/
/*Button Press Function*/
char b1()
    {
    fd = open("/sys/class/gpio/gpio44/value", O_RDONLY); /*Opens GPIO value file*/
    read(fd, buf, 1); /*Reads the GPIO's 12 PIN value and stores it*/
    int a = atof(buf);
    close(fd); /*Closes the GPIO file*/
    return a; /*Function returns the stored GPIO value*/

    } /*Function ends*/
void webpage() ;
/*Main Function*/
int main()
    {

    while (c != 7)	/*Counts until 7 keys*/
	{
	sleep(1);
	if (b1() != 0)		/*Button 1Press */
	    {
	    c++; /* Counts the button press*/
	    d++; /* Counts the right button press*/
	    e[i++] = d; /* stores the right button press count*/
	    if (c == 2)
		{
		d++; //break;
		e[i++] = d;
		}
	    if (c == 3)
		{
		d++; //break;
		e[i++] = d;
		}
	    if (c == 7)
		{
		d++; //break;
		e[i++] = d;
		}
	    } /*Conditional loop ends*/

	if (b2() != 0)		/*Button 2 Press */
	    {
	    c++; /* Counts the button press*/
	    d++; /* Counts the right button press*/
	    e[i++] = d; /* stores the right button press count*/
	    if (c == 1)
		{
		d++; //break;
		e[i++] = d;
		}
	    if (c == 4)
		{
		d++; //break;
		e[i++] = d;
		}
	    if (c == 5)
		{
		d++; //break;
		e[i++] = d;
		}
	    if (c == 6)
		{
		d++; //break;
		e[i++] = d;
		}
	    } /*Conditional loop ends*/
	if (b1() != 0 || b2() != 0)
	    {
	    printf("* ");
	    //printf("\nD: %d ", i);
	    }

	fflush(stdout); /*This is required to work the sleep()*/
	} /*forever loop ends*/
    if (i== 7){
  		printf("\nACCESS OK\n");
  		//system("echo out > /home/debian/site/keypass");
  		printf("\nKey is created!\n\n\n");
  		webpage();
    }
    else
	printf("\nACCESS DENIED!\n");
    return 0;
    } /*Main ends*/

/*Date and Time Function*/
const char* td() {
  time_t      t;
  struct tm*  ptm;
  t = time(NULL);
  ptm = localtime(&t);
  strftime(cur_time, 128, "%d-%b-%Y %H:%M:%S", ptm);
  //printf("Current date and time: %s\n", cur_time);
  return cur_time;
  }

void webpage() {
td();
    /*------------------------------------------------------*/
    	/*EDIT THE webpage*/
    	/*------------------------------------------------------*/
    	system("sed -i '$ d' /home/debian/site/keyaccess.html >/dev/null 2>&1");//deletes the last line
    	FILE *webpage = fopen("/home/debian/site/keyaccess.html", "a");		//opens the webpage
    	if (!webpage) {
    		//handle the error
    		printf("something went wrong: %s", strerror(errno));
    		exit(1);
    	}
    	/*---------------------Creates the HTML code---------------------*/
    	char plink[100] = "<p>Access Time and date: ";
    	strncat(plink, cur_time,50);
    	strncat(plink,"</p>",50);
    	fprintf(webpage, "\n%s\n", plink);
    	fprintf(webpage,"\n%s", "</div> <div class=\"footer\"> <p>Designed by Srikanth Pendem</p> </div></body> </html>"); //appends the footer and ends the HTML
    	//printf("\n%s\n",plink);
    	fclose(webpage);				//close the webpage
    	/*------------------------------------------------------*/
    	/*UPLOAD THE webpage*/
    	/*------------------------------------------------------*/
    	system("aws s3 cp /home/debian/site/keyaccess.html s3://biometricbin2020/ --acl public-read >/dev/null 2>&1");			//This command uploads the webpage to cloud
    	printf(
    			"\n\nUpload Done\nPlease visit our website https://biometricbin2020.s3.ca-central-1.amazonaws.com/index.html for more info\n\n");
    }
