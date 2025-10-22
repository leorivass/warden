#include "i2c.h"
#include "mpu6050.h"

static const float gyro_lsb_sensitivity  = 65.5;
static const float accel_lsb_sensitivity = 16384.0;

void mpu6050_init() {

    i2c_init();

    /* Note: The device will come up in sleep mode upon power-up. */
    /* Turn off power sleep mode and cycle mode */
    i2c_write_register(MPU6050_ADDRESS, PWR_MGMT_1_REG, 0x0);

    /* Set full scale range of the gyroscope to +-500°/s */
    i2c_write_register(MPU6050_ADDRESS, GYRO_CONFIG_REG, (0x01 << 3));

    /* Set full scale range of the accelerometer to +-2g */
    i2c_write_register(MPU6050_ADDRESS, ACCEL_CONFIG_REG, 0x0);

    return;
}

int get_mpu6050_data(mpu6050_data *data) {

    /* Evaluate if data pointer is NULL */
    if (data == NULL) {
        return -1;
    }
    
    uint8_t  gyro_xout_h_raw,  gyro_xout_l_raw;
    uint8_t  gyro_yout_h_raw,  gyro_yout_l_raw;
    uint8_t  gyro_zout_h_raw,  gyro_zout_l_raw;

    uint8_t accel_xout_h_raw, accel_xout_l_raw;
    uint8_t accel_yout_h_raw, accel_yout_l_raw;
    uint8_t accel_zout_h_raw, accel_zout_l_raw;

    int16_t  raw_gyro_x,  raw_gyro_y,  raw_gyro_z;
    int16_t raw_accel_x, raw_accel_y, raw_accel_z;

    /* Get gyroscope values */
    i2c_read_register(MPU6050_ADDRESS, GYRO_XOUT_H_REG, &gyro_xout_h_raw);
    i2c_read_register(MPU6050_ADDRESS, GYRO_XOUT_L_REG, &gyro_xout_l_raw);
    i2c_read_register(MPU6050_ADDRESS, GYRO_YOUT_H_REG, &gyro_yout_h_raw);
    i2c_read_register(MPU6050_ADDRESS, GYRO_YOUT_L_REG, &gyro_yout_l_raw);
    i2c_read_register(MPU6050_ADDRESS, GYRO_ZOUT_H_REG, &gyro_zout_h_raw);
    i2c_read_register(MPU6050_ADDRESS, GYRO_ZOUT_L_REG, &gyro_zout_l_raw);

    /* Get accelerometer values */
    i2c_read_register(MPU6050_ADDRESS, ACCEL_XOUT_H_REG, &accel_xout_h_raw);
    i2c_read_register(MPU6050_ADDRESS, ACCEL_XOUT_L_REG, &accel_xout_l_raw);
    i2c_read_register(MPU6050_ADDRESS, ACCEL_YOUT_H_REG, &accel_yout_h_raw);
    i2c_read_register(MPU6050_ADDRESS, ACCEL_YOUT_L_REG, &accel_yout_l_raw);
    i2c_read_register(MPU6050_ADDRESS, ACCEL_ZOUT_H_REG, &accel_zout_h_raw);
    i2c_read_register(MPU6050_ADDRESS, ACCEL_ZOUT_L_REG, &accel_zout_l_raw);

    /* Get 16-bit raw values for each axis */
    raw_gyro_x  = (int16_t)((gyro_xout_h_raw << 8) | gyro_xout_l_raw);
    raw_gyro_y  = (int16_t)((gyro_yout_h_raw << 8) | gyro_yout_l_raw);
    raw_gyro_z  = (int16_t)((gyro_zout_h_raw << 8) | gyro_zout_l_raw);

    raw_accel_x = (int16_t)((accel_xout_h_raw << 8) | accel_xout_l_raw);
    raw_accel_y = (int16_t)((accel_yout_h_raw << 8) | accel_yout_l_raw);
    raw_accel_z = (int16_t)((accel_zout_h_raw << 8) | accel_zout_l_raw);

    /* Store data */
    data->gyro_xout = raw_gyro_x / gyro_lsb_sensitivity;
    data->gyro_yout = raw_gyro_y / gyro_lsb_sensitivity;
    data->gyro_zout = raw_gyro_z / gyro_lsb_sensitivity;

    data->accel_xout = raw_accel_x / accel_lsb_sensitivity;
    data->accel_yout = raw_accel_y / accel_lsb_sensitivity;
    data->accel_zout = raw_accel_z / accel_lsb_sensitivity;

    return 0;
}

