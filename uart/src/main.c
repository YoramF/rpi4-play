// This testing program is base on:
// Adam Greenwood-Byrne (isometimes) with his project rpi4-osdev.

#include <stdio.h>
#include "io.h"

int main () {
	printf("UART demo program\n\
It usees mmap() to map to /dev/mem and access peripheral registers\n\
Type any anything and the program will display what you typed in\n\
use ^D to exit\n");
	printf("All input output will be sent to putty terminal connected to\n\
raspberry via GPIO 2 USB cable\n");
	uart_init();
	uart_writeText("\
UART demo program\n\
It usees mmap() to map to /dev/mem and access peripheral registers\n\
Type any anything and the program will display what you typed in\n\
use ^D to exit\n");

	while(uart_update());

	uart_close();
}