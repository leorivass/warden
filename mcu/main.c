#include "gpio.h"
#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main() {

    cli();
    serial_begin(9600);
    sei();

    while (1) {
        serial_println("Hi, I'm testing this driver with screen terminal");
        _delay_ms(1000);
        serial_println("It seems to work properly!");
        _delay_ms(1000);
    }

    return 0;
}