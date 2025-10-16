#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"

#define UBRR_VALUE (F_CPU / 16 / baud_rate - 1)
#define BUFFER_SIZE 128

/* Buffer to store characters that will be transmitted */
char tx_buffer[BUFFER_SIZE];
/* Writing index */
volatile int main_index = 0; 
/* Reading index */
volatile int interrupt_index = 0; 

ISR(USART0_UDRE_vect) {
    if (interrupt_index < main_index) {
        UDR0 = tx_buffer[interrupt_index++];
    } else {
        UCSR0B &= ~(1 << UDRIE0);   /* Disable interrupt */
        interrupt_index = 0;
        main_index = 0;
    }
}

/* Function that initializes the serial port with a specific baud rate */
int serial_begin(uint16_t baud_rate) {

    /* Set baud rate */
    UBRR0L = (uint8_t) UBRR_VALUE;
    UBRR0H = (uint8_t) (UBRR_VALUE >> 8);

    /* Set frame format: 8data, 1stop bit */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    return 0;
}

/* Function to transmit a character through serial port */
int serial_transmit(char data) {
    
    /* tx_buffer is full */
    if (main_index >= BUFFER_SIZE) return -1;

    tx_buffer[main_index++] = data;

    /* Enable Data Register Empty Interrupt Enable */
    UCSR0B = UCSR0B | (1 << UDRIE0);

    return 0;
}

int serial_println(const char *format, ...) {

    char user_buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, format); 
    
    vsprintf(user_buffer, format, args); 
    va_end(args); 

    for (int i = 0; user_buffer[i] != '\0'; i++) {
        serial_transmit(user_buffer[i]);
    }

    serial_transmit('\r');
    serial_transmit('\n');

    return 0;
}