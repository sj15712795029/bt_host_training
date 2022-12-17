#include <stdio.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


int uart_fd = -1;
struct termios toptions;


void uart_bt_open()
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

void uart_bt_close()
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

void test_send_recv()
{
	int read_len = 0;
	uint8_t hci_evt_buf[128];
	uint8_t hci_reset[] = {0x01,0x03,0x0c,0x00};

	for(int index = 0; index < 10; index++)
	{
		uart_bt_send(hci_reset,sizeof(hci_reset));
		printf("send ....\r\n");
		sleep(1);
	}
	
	
	read_len = uart_bt_read(hci_evt_buf,sizeof(hci_evt_buf));
	printf("read len:%d\r\n",read_len);
	for(int index = 0; index < read_len; index++)
		printf("0x%02x ",hci_evt_buf[index]);

	printf("\r\n");
}

int main()
{
	uart_bt_open();
	
	test_send_recv();

	while(1)
		sleep(1);

    return 0;
}
