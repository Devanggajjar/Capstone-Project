## GPS Interfacing with the Beaglebone.
### This directory contains the source code of the GPS Data, parsing from Adafruit GPS Breakout v3 and uploads the data log into the website.
- The whole source code is written in C only.
- This source code only parses the GPRMC string from NMEA Strings.
  - GPGGA string parsing is also possible by adding and eliminating some lines in the code.
 - This code uses math libraries, do not forget to add ```-lm``` at the end of the compilation command.
 - Documentation of the code is within the comments
 - Please update your linux machine before running the code.
 - This code edits an ```HTML file```, Make sure you have ```sed``` version above 3.5 on the linux.
 ### Just run the executable located at the Debug directory.
```console
./GPS
```

