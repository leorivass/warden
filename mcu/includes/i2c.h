#ifndef I2C_H
#define I2C_H

#include <stdio.h>

#define START_STATUS_CODE       0x08
#define REPEATED_START_CODE     0x10    
#define MT_SLA_ACK              0x18
#define MT_DATA_ACK             0x28
#define MR_SLA_ACK              0x40
#define MR_DATA_ACK             0x50
#define MR_DATA_NACK            0x58

typedef enum {
    I2C_OK,
    I2C_ERR_NULL_PTR,
    I2C_ERR_START,
    I2C_ERR_REPEATED_START,
    I2C_ERR_SLAVE_WRITE,
    I2C_ERR_SLAVE_READ,
    I2C_ERR_WRITE_DATA,
    I2C_ERR_READ_ACK,
    I2C_ERR_READ_NACK, 
    I2C_ERR_INVALID_LEN,
    I2C_ERR_UNKNOWN
} i2c_error_t;

void i2c_init();
int i2c_error_tag(uint8_t status_code);
int i2c_check_ack(uint8_t status_code);
int i2c_send_register(uint8_t slave_address, uint8_t reg_address);
int i2c_send_repeated_start(uint8_t slave_address);
int i2c_write_register(uint8_t slave_address, uint8_t reg_address, uint8_t data);
int i2c_read_register(uint8_t slave_address, uint8_t reg_address, uint8_t *data);
int i2c_burst_read_registers(uint8_t slave_address, uint8_t start_reg, uint8_t *buffer, uint8_t length);

#endif