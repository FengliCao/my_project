/*================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   
*   文件名称：uart.h
*   创 建 者：FengliCao
*   创建日期：2018年04月30日
*   描    述：
*
================================================================*/


#ifndef _UART_H
#define _UART_H
const int RESET =1;
const int ONE_GET = 2;

int init_uart(void);
int m_write(int,int);
int m_read(int);

#endif //UART_H
