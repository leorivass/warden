#ifndef I2C_H
#define I2C_H

#include <stdio.h>

void i2c_init();
int i2c_write_register(uint8_t, uint8_t, uint8_t);
int i2c_read_register(uint8_t, uint8_t, uint8_t*);
int i2c_burst_read_registers(uint8_t, uint8_t, uint8_t*, uint8_t);

#endif