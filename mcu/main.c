#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "usart.h" // Just for debugging
#include "mpu6050.h"

int main() {

    // uint8_t who_am_i;
    mpu6050_data data;

    cli();
    serial_begin(9600);
    mpu6050_init();
    sei();

    // i2c_read_register(MPU6050_ADDRESS, WHO_AM_I_REG, &who_am_i);
    // serial_println("El valor del registro who_am_i es: %d", who_am_i);

    while(1) {

    get_mpu6050_data(&data);
    
    serial_println(" Gyro_x: %.2f;  Gyro_y: %.2f ;  Gyro_z: %.2f", data.gyro_xout, data.gyro_yout, data.gyro_zout);
    serial_println("Accel_x: %.2f ; Accel_y: %.2f ; Accel_z: %.2f\n", data.accel_xout, data.accel_yout, data.accel_zout);

    _delay_ms(1000);

    }

    return 0;
}