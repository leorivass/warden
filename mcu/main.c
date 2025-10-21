#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "usart.h" // Just for debugging
#include "i2c.h"

#define MPU6050_ADDRESS     0x68
#define WHO_AM_I_REG        0x75

int main() {

    uint8_t who_am_i;

    cli();
    serial_begin(9600);
    i2c_init();
    sei();

    i2c_read_register(MPU6050_ADDRESS, WHO_AM_I_REG, &who_am_i);
    serial_println("El valor del registro who_am_i es: %d", who_am_i);
    
    while(1);

    return 0;
}