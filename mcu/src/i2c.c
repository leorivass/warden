#include <avr/io.h>
// #include "usart.h"

#define START_STATUS_CODE       0x08
#define REPEATED_START_CODE     0x10    
#define MT_SLA_ACK              0x18
#define MT_DATA_ACK             0x28
#define MR_SLA_ACK              0x40
#define MR_DATA_ACK             0x50
#define MR_DATA_NACK            0x58

#define send_start()\
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN)
#define send_stop()\
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN)
#define set_twint()\
    TWCR = (1 << TWINT) | (1 << TWEN)
#define send_ack()\
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA)

void i2c_init() {
    /* Set I2C transmission speed to 400Khz (fast mode) */
    /* Bits TWPS1 and TWPS0 are set to 0 by default, so we are not going to touch them to have a prescaler value of 1 */
    TWBR = 12;

    return;
}

int i2c_write_register(uint8_t slave_address, uint8_t reg_address, uint8_t data) {

    send_start();

    /* Wait for TWINT flag is set by hardware to start transmission */
    while(!(TWCR & (1 << TWINT)));

    /* If the status code is not 0x08 after sending send_start(), then there was an error*/
    if ((TWSR & 0xF8) != START_STATUS_CODE) {
        send_stop();
        return -1;
    }
        
    /* Load slave address + WRITE bit into TWDR register */ 
    TWDR = slave_address << 1;

    /* Set TWINT to 1 to clear the TWINT flag and start transmission */
    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x20 after sending SLAVE_W, NOT_ACK has been received */
    /* If status code == 0x18, ACK has been received */
    if ((TWSR & 0xF8) != MT_SLA_ACK) {
        send_stop(); 
        return -2;
    }
 
    TWDR = reg_address;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x30 after sending DATA, NOT_ACK has been received */
    /* If status code == 0x28, ACK has been received */
    if ((TWSR & 0xF8) != MT_DATA_ACK) {
        send_stop();
        return -3;
    }

    TWDR = data;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != MT_DATA_ACK) {
        send_stop();
        return -4;
    }

    send_stop();

    return 0;
}

int i2c_read_register(uint8_t slave_address, uint8_t reg_address, uint8_t *data) {

    send_start();

    /* Wait for TWINT flag is set by hardware to start transmission */
    while(!(TWCR & (1 << TWINT)));

    /* If the status code is not 0x08 after sending send_start(), then there was an error*/
    if ((TWSR & 0xF8) != START_STATUS_CODE) {
        send_stop();
        return -1;
    }
        
    /* Load slave address + WRITE bit into TWDR register */ 
    TWDR = slave_address << 1;

    /* Set TWINT to clear the TWINT flag and start transmission */
    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x20 after sending SLAVE_W, NOT_ACK has been received */
    /* If status code == 0x18, ACK has been received */
    if ((TWSR & 0xF8) != MT_SLA_ACK) {
        send_stop(); 
        return -2;
    }
 
    TWDR = reg_address;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x30 after sending DATA, NOT_ACK has been received */
    /* If status code == 0x28, ACK has been received */
    if ((TWSR & 0xF8) != MT_DATA_ACK) {
        send_stop();
        return -3;
    }

    /* Send a repeated start condition to read the register */
    send_start();

    while(!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != REPEATED_START_CODE) {
        send_stop();
        return -4;
    }

    /* Load slave address + READ bit into TWDR register */
    TWDR = (slave_address << 1) | 0x01;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != MR_SLA_ACK) {
        send_stop();
        return -5;
    }

    /* Read one byte, send NACK */
    set_twint();

    while(!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != MR_DATA_NACK) {
        send_stop();
        return -6;
    }

    *data = TWDR;

    send_stop();

    return 0;
}

int i2c_burst_read_registers(uint8_t slave_address, uint8_t start_reg, uint8_t *buffer, uint8_t length) {

    send_start();

    /* Wait for TWINT flag is set by hardware to start transmission */
    while(!(TWCR & (1 << TWINT)));

    /* If the status code is not 0x08 after sending send_start(), then there was an error*/
    if ((TWSR & 0xF8) != START_STATUS_CODE) {
        send_stop();
        return -1;
    }

    /* Load slave address + WRITE bit into TWDR register */ 
    TWDR = slave_address << 1;

    /* Set TWINT to clear the TWINT flag and start transmission */
    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x20 after sending SLAVE_W, NOT_ACK has been received */
    /* If status code == 0x18, ACK has been received */
    if ((TWSR & 0xF8) != MT_SLA_ACK) {
        send_stop(); 
        return -2;
    }

    TWDR = start_reg;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x30 after sending DATA, NOT_ACK has been received */
    /* If status code == 0x28, ACK has been received */
    if ((TWSR & 0xF8) != MT_DATA_ACK) {
        send_stop();
        return -3;
    }

    /* Send a repeated start condition to read the register */
    send_start();

    while(!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != REPEATED_START_CODE) {
        send_stop();
        return -4;
    }

    /* Load slave address + READ bit into TWDR register */
    TWDR = (slave_address << 1) | 0x01;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != MR_SLA_ACK) {
        send_stop();
        return -5;
    }

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

        while(!(TWCR & (1 << TWINT)));

        if ((TWSR & 0xF8) != STATUS_CODE) {
            send_stop();
            return -6;
        }

        buffer[i] = TWDR;

    }

    send_stop();

    return 0;
}