#include "btsnoop.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>

int btsnoop_fd = -1;


void be_store_32(uint8_t *buffer,uint32_t value)
{
    uint8_t index = 0;
    buffer[index++] = (uint8_t)(value >> 24);
    buffer[index++] = (uint8_t)(value >> 16);
    buffer[index++] = (uint8_t)(value >> 8);
    buffer[index++] = (uint8_t)(value);
}

void be_store_64(uint8_t *buffer,uint64_t value)
{
    uint8_t index = 0;
    buffer[index++] = (uint8_t)(value >> 56);
    buffer[index++] = (uint8_t)(value >> 48);
    buffer[index++] = (uint8_t)(value >> 40);
    buffer[index++] = (uint8_t)(value >> 32);
    buffer[index++] = (uint8_t)(value >> 24);
    buffer[index++] = (uint8_t)(value >> 16);
    buffer[index++] = (uint8_t)(value >> 8);
    buffer[index++] = (uint8_t)(value);
}


uint8_t btsnoop_open(uint8_t *file_naname)
{
    if(!file_naname)
        return 1;

    btsnoop_fd = open(file_naname,O_CREAT | O_TRUNC | O_WRONLY);
    if(btsnoop_fd == -1)
        return 1;

    write(btsnoop_fd,"btsnoop\0\0\0\0\1\0\0\x3\xea", 16);

}
uint8_t btsnoop_close(void)
{
    if(btsnoop_fd != -1)
    {
        close(btsnoop_fd);
        btsnoop_fd = -1;
    }
}

uint8_t btsnoop_write(uint8_t type,uint8_t in,uint8_t *data,uint16_t data_len)
{
    uint32_t orig_len;
    uint32_t include_len;
    uint32_t flags,temp_flag;
    uint32_t drop;
    uint64_t timestamp;

    if(type == TRANSPORT_TYPE_CMD)
        temp_flag = 2;
    else if(type == TRANSPORT_TYPE_ACL || type == TRANSPORT_TYPE_SCO)
        temp_flag = in;
    else if(type == TRANSPORT_TYPE_EVT)
        temp_flag = 3;

    struct timeval curr_time;
    gettimeofday(&curr_time,NULL);

    be_store_32((uint8_t *)&orig_len,data_len);
    be_store_32((uint8_t *)&include_len,data_len);
    be_store_32((uint8_t *)&flags,temp_flag);
    be_store_32((uint8_t *)&drop,0);
    be_store_64((uint8_t *)&timestamp,curr_time.tv_usec);


    write(btsnoop_fd,&orig_len,sizeof(orig_len));
    write(btsnoop_fd,&include_len,sizeof(include_len));
    write(btsnoop_fd,&flags,sizeof(flags));
    write(btsnoop_fd,&drop,sizeof(drop));
    write(btsnoop_fd,&timestamp,sizeof(timestamp));

    write(btsnoop_fd,data,data_len);



}

