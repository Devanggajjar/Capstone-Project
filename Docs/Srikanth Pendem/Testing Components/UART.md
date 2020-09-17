**UART1 :: /dev/ttyO1**
```shell
config-pin P9.26 uart #RX
config-pin P9.24 uart #TX
```
**UART4 :: /dev/ttyO4**  
```shell
config-pin P9.11 uart #RX
config-pin P9.13 uart #TX
```
**UART2 :: /dev/ttyO2** 
```shell
config-pin P9.21 uart #TX
config-pin P9.22 uart #RX
```
```shell
minicom -b 9600 -D /dev/ttyO1 #UART1 Transmission check.
minicom -b 9600 -D /dev/ttyO2 #UART2 Transmission check.
minicom -b 9600 -D /dev/ttyO4 #UART4 Transmission check.
```
```shell
config-pin P9.26 uart #RX 
config-pin P9.24 uart #TX 
config-pin P9.21 uart #TX 
config-pin P9.22 uart #RX 
```
