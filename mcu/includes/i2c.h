#ifndef I2C_H
#define I2C_H

#include <stdio.h>

void i2c_init();
int i2c_write(uint8_t, uint8_t);

#endif