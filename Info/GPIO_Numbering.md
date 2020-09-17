### GPIO Pin Names, GPIO Pin Numbers
- Physical pin numbers on the BBB have names.
- You can check the list of available GPIOs with their names by the command ***ls /sys/class/gpio***
- Physical pin numbers may have named as GPIO01_XX or GPIO02_XX or GPIO03_XX
- To appropriate Physical Pin Names with the BBB Pin names, you can calculate
- GPIO Pin name on BBB = 32x1(or)2(or)3+XX Physical GPIO Pin name.
