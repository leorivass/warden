#include <avr/io.h>

#define START_STATUS_CODE   0x08
#define MT_SLA_ACK          0x18
#define MT_DATA_ACK         0x28

#define send_start()\
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN)
#define send_stop()\
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN)
#define set_twint()\
    TWCR = (1 << TWINT) | (1 << TWEN)

void i2c_init() {
    /* Set I2C transmission speed to 400Khz (fast mode) */
    /* Bits TWPS1 and TWPS0 are set to 0 by default, so we are not going to touch them to have a prescaler value of 1 */
    TWBR = 12;

    return;
}

int i2c_write(uint8_t slave_address, uint8_t data) {

    send_start();

    /* Wait for TWINT flag is set by hardware to start transmission */
    while(!(TWCR & (1 << TWINT)));

    /* If the status code is not 0x08 after sending send_start(), then there was an error*/
    if ((TWSR & 0xF8) != START_STATUS_CODE) {
        send_stop();
        return -1;
    }
        
    /* Load slave address + R/W bit into TWDR register */ 
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
 
    TWDR = data;

    set_twint();

    while(!(TWCR & (1 << TWINT)));

    /* If the status code is 0x30 after sending SLAVE_W, NOT_ACK has been received */
    /* If status code == 0x28, ACK has been received */
    if ((TWSR & 0xF8) != MT_DATA_ACK) {
        send_stop();
        return -3;
    }

    send_stop();

    return 0;
}