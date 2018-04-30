/*================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   
*   文件名称：uart.c
*   创 建 者：FengliCao
*   创建日期：2018年04月30日
*   描    述：
*
================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include "uart.h"

#define BUFF_SIZE 10
int main()
{
	int fd;
	fd = init_uart();
    close(fd);
	return 0;

}
int init_uart()
{
	int fd;
	struct termios opt;

	fd = open("/dev/ttySAC3", O_RDWR);
	if (-1 == fd)
	{
		perror("open tty_dev");
		return 0;
	}
	printf("Open tty_dev Ok !\n");

	tcgetattr(fd, &opt);
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);

	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;

	opt.c_cflag &= ~PARENB;
	opt.c_iflag &= ~INPCK;
	
	opt.c_lflag &= ~ICANON;
	opt.c_lflag &= ~ECHO;
/* 1 tingzhi wei*/
	opt.c_cflag &= ~CSTOPB;

	opt.c_cc[VTIME] = 10;
	opt.c_cc[VMIN] = 0;
	if( 0 != tcsetattr(fd, TCSANOW, &opt))
	{
		perror("set b");
		return 0;
	}
	tcflush(fd, TCIOFLUSH);
	m_write(fd, 1112);
	return fd;
}

int m_read(int fd)
{
	int n;
	char buffer[BUFF_SIZE] = {0};
		n = read(fd, buffer, BUFF_SIZE);
		if(0 >= n) 
		{
			perror("read date");
		}
		buffer[n] = '\0';

		printf("%s \n", buffer);

		return n;
}
int m_write(int fd, int date)
{
	char wbuffer[BUFF_SIZE];
    sprintf((char *)wbuffer,"%d",date);
	if(-1 == write(fd, wbuffer, strlen(wbuffer)))
	{
		perror("write date");
		return -1;
	}
    return 0;
}






















