#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "uart.h"

typedef enum
{
	BT_H4_TYPE_CMD = 0x01,
	BT_H4_TYPE_ACL = 0x02,
	BT_H4_TYPE_SCO = 0x03,
	BT_H4_TYPE_EVT = 0x04,
	BT_H4_TYPE_ISO = 0x04,
}bt_h4_type_t;

typedef enum
{
	BT_H4_W4_TRANSPORT_TYPE,
	BT_H4_W4_EVT_HDR,
	BT_H4_W4_ACL_HDR,
	BT_H4_W4_EVT_PARAM,
	BT_H4_W4_ACL_PARAM,
}bt_h4_read_status_t;

uint16_t read_pos = 0;
bt_h4_read_status_t h4_read_status = BT_H4_W4_TRANSPORT_TYPE;

uint16_t event_param_len;
uint16_t acl_param_len;
uint8_t bt_rx_buffer[1024] = {0};


void bt_evt_data_process(uint8_t *data,uint16_t data_len)
{
	printf("bt_evt_data_process\r\n");
	printf("-------------------------------------\r\n");
	for(int index = 0; index < data_len; index++)
		printf("0x%02x ",data[index]);
	printf("\r\n-------------------------------------\r\n");
}
	

int main()
{
	uint8_t hci_reset[] = {0x01,0x03,0x0c,0x00};
	uart_bt_open();

	for(int index = 0; index < 10; index++)
	{
		uart_bt_send(hci_reset,sizeof(hci_reset));
		printf("send ....\r\n");
		sleep(1);
	}
	

	while(1)
	{
		switch(h4_read_status)
		{
			case BT_H4_W4_TRANSPORT_TYPE:
				uart_bt_read(bt_rx_buffer,1);
				if(bt_rx_buffer[0] == BT_H4_TYPE_EVT)
					h4_read_status = BT_H4_W4_EVT_HDR;
				else if(bt_rx_buffer[0] == BT_H4_TYPE_ACL)
					h4_read_status = BT_H4_W4_ACL_HDR;

				read_pos = 1;
				break;
			case BT_H4_W4_EVT_HDR:
				uart_bt_read(bt_rx_buffer+read_pos,2);
				printf("event code:0x%x len:%d\r\n",bt_rx_buffer[read_pos],bt_rx_buffer[read_pos+1]);
				event_param_len = bt_rx_buffer[read_pos+1];
				h4_read_status = BT_H4_W4_EVT_PARAM;
				read_pos += 2;
	
				break;
			case BT_H4_W4_ACL_HDR:
				uart_bt_read(bt_rx_buffer+read_pos,4);

				h4_read_status = BT_H4_W4_ACL_PARAM;
				read_pos += 4;
				break;
			case BT_H4_W4_EVT_PARAM:
				uart_bt_read(bt_rx_buffer+read_pos,event_param_len);
				/* Event process */
				bt_evt_data_process(bt_rx_buffer,read_pos+event_param_len);
				
				read_pos = 0;
				h4_read_status = BT_H4_W4_TRANSPORT_TYPE;
				break;
			case BT_H4_W4_ACL_PARAM:
				break;
			default:
				break;
		}
	}

	uart_bt_close();

    return 0;
}
