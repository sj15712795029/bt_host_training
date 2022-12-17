#include "uart.h"
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


int uart_fd = -1;
struct termios toptions;


void uart_bt_open(void)
{
    uart_fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);
    if(uart_fd < 0)
    {
        printf("can not open uart tty port\r\n");
        return ;
    }

	if(tcgetattr(uart_fd, &toptions) <0)
	{
		printf("can not get terminal attributes\r\n");
		return;
	}

	cfmakeraw(&toptions);

	toptions.c_cflag |= CRTSCTS | CREAD | CLOCAL;

	cfsetispeed(&toptions,B115200);
	cfsetospeed(&toptions,B115200);

	if(tcsetattr(uart_fd,TCSANOW, &toptions) <0)
	{
		printf("can not get terminal attributes\r\n");
		return;
	}
	


}

void uart_bt_close(void)
{
    if(uart_fd > 0)
        close(uart_fd);

        uart_fd = -1;

}

int uart_bt_send(uint8_t *data, uint16_t len)
{
    return write(uart_fd,data,len);
}

int uart_bt_read(uint8_t *data, uint16_t len)
{
    return read(uart_fd,data,len);
}

