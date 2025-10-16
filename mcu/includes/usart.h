#ifndef USART_H
#define USART_H

#define F_CPU 16000000UL

int serial_begin(uint16_t);
int serial_transmit(char);
int serial_println(const char*, ...);

#endif