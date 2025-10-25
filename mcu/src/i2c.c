#include <avr/io.h>
#include "i2c.h"

#define I2C_READ_BIT 0x01

#define send_start()\
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN)
#define send_stop()\
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN)
#define set_twint()\
    TWCR = (1 << TWINT) | (1 << TWEN)
#define send_ack()\
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA)
#define wait_twint_flag()\
    while(!(TWCR & (1 << TWINT)))

int i2c_error_tag(uint8_t status_code) {

    switch(status_code) {
        case START_STATUS_CODE: return I2C_ERR_START;
        case REPEATED_START_CODE: return I2C_ERR_REPEATED_START;
        case MT_SLA_ACK: return I2C_ERR_SLAVE_WRITE;
        case MT_DATA_ACK: return I2C_ERR_WRITE_DATA;
        case MR_SLA_ACK: return I2C_ERR_SLAVE_READ;
        case MR_DATA_ACK: return I2C_ERR_READ_ACK;
        case MR_DATA_NACK: return I2C_ERR_READ_NACK;
    }

    return I2C_ERR_UNKNOWN;
}

int i2c_check_ack(uint8_t status_code) {

    if ((TWSR & 0xF8) != status_code) {
        send_stop();
        return -i2c_error_tag(status_code);
    }

    return I2C_OK;
}

void i2c_init() {
    /* Set I2C transmission speed to 400Khz (fast mode) */
    /* Bits TWPS1 and TWPS0 are set to 0 by default, so we are not going to touch them to have a prescaler value of 1 */
    TWBR = 12;
}

int i2c_send_register(uint8_t slave_address, uint8_t reg_address) {

    int ret;

    send_start();
    wait_twint_flag();

    ret = i2c_check_ack(START_STATUS_CODE);
    if(ret) return ret;
     
    /* Load slave address + WRITE bit into TWDR register */ 
    TWDR = slave_address << 1;

    set_twint();
    wait_twint_flag();

    ret = i2c_check_ack(MT_SLA_ACK);
    if(ret) return ret;
 
    TWDR = reg_address;

    set_twint();
    wait_twint_flag();

    ret = i2c_check_ack(MT_DATA_ACK);
    if(ret) return ret;  

    return I2C_OK;
}

int i2c_send_repeated_start(uint8_t slave_address) {

    int ret;
    
    /* Send a repeated start condition to read the register */
    send_start();
    wait_twint_flag();

    ret = i2c_check_ack(REPEATED_START_CODE);
    if(ret) return ret;

    /* Load slave address + READ bit into TWDR register */
    TWDR = (slave_address << 1) | I2C_READ_BIT;

    set_twint();
    wait_twint_flag();

    ret = i2c_check_ack(MR_SLA_ACK);
    if(ret) return ret;

    return I2C_OK;
}

int i2c_write_register(uint8_t slave_address, uint8_t reg_address, uint8_t data) {

    int ret;

    ret = i2c_send_register(slave_address, reg_address);
    if(ret) return ret;

    TWDR = data;

    set_twint();
    wait_twint_flag();
    
    ret = i2c_check_ack(MT_DATA_ACK);
    if(ret) return ret;

    send_stop();

    return I2C_OK;
}

int i2c_read_register(uint8_t slave_address, uint8_t reg_address, uint8_t *data) {

    int ret;

    if (data == NULL) return -I2C_ERR_NULL_PTR;

    ret = i2c_send_register(slave_address, reg_address);
    if(ret) return ret;

    ret = i2c_send_repeated_start(slave_address);
    if(ret) return ret;

    /* Read one byte, send NACK */
    set_twint();
    wait_twint_flag();

    ret = i2c_check_ack(MR_DATA_NACK);
    if(ret) return ret;

    *data = TWDR;

    send_stop();

    return I2C_OK;
}

int i2c_burst_read_registers(uint8_t slave_address, uint8_t start_reg, uint8_t *buffer, uint8_t length) {

    int ret;

    if (buffer == NULL) return -I2C_ERR_NULL_PTR;
    if (length == 0) return -I2C_ERR_INVALID_LEN;

    ret = i2c_send_register(slave_address, start_reg);
    if(ret) return ret;

    ret = i2c_send_repeated_start(slave_address);
    if(ret) return ret;

    for(uint8_t i = 0; i < length; i++) {

        uint8_t STATUS_CODE;

        if (i == (length - 1)) {
            /* Read one byte and send NACK, for this is the last reading */
            set_twint();
            STATUS_CODE = MR_DATA_NACK;
        }
        else {
            /* Read one byte and send ACK, for we want to keep reading */
            send_ack();
            STATUS_CODE = MR_DATA_ACK;
        }

        wait_twint_flag();

        ret = i2c_check_ack(STATUS_CODE);
        if(ret) return ret;

        buffer[i] = TWDR;

    }

    send_stop();

    return I2C_OK;
}