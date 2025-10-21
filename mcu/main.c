#include <avr/io.h>
#include <util/delay.h>
// #include <avr/interrupt.h> 
// #include "usart.h" // Just for debugging
#include "i2c.h"

#define LCD_ADDRESS 0x27
#define TURN_OFF_BACKLIGHT 0x4
#define TURN_ON_BACKLIGHT  0xC

int main() {

    int ret;

    // cli();
    // serial_begin(9600);
    i2c_init();
    // sei();

    i2c_write(LCD_ADDRESS, TURN_OFF_BACKLIGHT);
    i2c_write(LCD_ADDRESS, 0x0);
    
    while(1);

    return 0;
}