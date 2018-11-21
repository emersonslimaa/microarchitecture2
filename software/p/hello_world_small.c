#include "sys/alt_stdio.h"
#include "system.h"
#include "altera_avalon_uart_regs.h"
#include "io.h"
#include <string.h>
#define NUMTRY 5

void delay(int a) { //bounce
	volatile int i = 0;
	while (i < a * 10000) {
		i++;
	}
}

void write(char * v) {
	int i = 0;
	while (v[i] != '\0') {
		IOWR_ALTERA_AVALON_UART_TXDATA(RS232_BASE, v[i]);
		delay(1);
		i++;
	}

}

void writeEsp(char * v) {
	int i = 0;
	while (v[i] != '\0') {
		IOWR_ALTERA_AVALON_UART_TXDATA(ESP_BASE, v[i]);
		//delay(1);
		i++;
	}

}

void espTest() {
	alt_putstr("AT\r\n");
	int i = 0;
	while (i < NUMTRY) {
		if (IORD_ALTERA_AVALON_UART_RXDATA(ESP_BASE) == 'K') {
			write("AT Ok\n");
			break;
		} else if (IORD_ALTERA_AVALON_UART_RXDATA(ESP_BASE) == 'E') {
			write("AT Error\n");
			alt_putstr("AT\r\n");
			i++;
		}
	}
	if (i == NUMTRY) {
		write("AT Fatal error");
	}
}

void espMode() {
	alt_putstr("AT+CWMODE=3\r\n");
	int i = 0;
	while (i < NUMTRY) {
		if (IORD_ALTERA_AVALON_UART_RXDATA(ESP_BASE) == 'K') {
			write("Mode Ok\n");
			break;
		} else if (IORD_ALTERA_AVALON_UART_RXDATA(ESP_BASE) == 'E') {
			write("Mode Error\n");
			alt_putstr("AT+CWMODE=3\r\n");
			i++;
		}
	}
	if (i == NUMTRY) {
		write("Mode Fatal Error\n");
	}
}

void espConnect(char * ssid, char * pass) {
	char a;
	char net[80] = "AT+CWJAP=\"";
	strcat(net, ssid);
	strcat(net, "\",\"");
	strcat(net, pass);
	strcat(net, "\"\r\n");

	alt_putstr(net);
	int i = 0;
	while (i < NUMTRY) {
		a = IORD_ALTERA_AVALON_UART_RXDATA(ESP_BASE);
		if (a == 'K') {
			write("Connect Ok\n");
			break;
		} else if (a == 'F'||a == 'E') {
			write("Connect Error\n");
			alt_putstr("AT+CWMODE=3\r\n");
			i++;
		}
	}
	if (i == NUMTRY) {
		write("Connect Fatal Error\n");
	}
}

void espInfo(char * cmd){
	alt_putstr(cmd);
	char a;
	do{
		a = IORD_ALTERA_AVALON_UART_RXDATA(ESP_BASE);
		/*if(a=='b'){
			write("ocupado");
			break;
		}*/
		IOWR_ALTERA_AVALON_UART_TXDATA(RS232_BASE, a);
	}while(a!='K');

}

int main() {
	espInfo("AT+CIFSR\r\n");
}
