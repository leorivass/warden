#include "i2c.h"
#include "mpu6050.h"

#define join_bytes(high, low)\
    (int16_t)((high << 8) | low)

static const float gyro_lsb_sensitivity  = 65.5f;
static const float accel_lsb_sensitivity = 16384.0f;

int mpu6050_init() {

    int ret;

    i2c_init();

    /* Note: The device will come up in sleep mode upon power-up. */
    /* Turn off power sleep mode and cycle mode */
    ret = i2c_write_register(MPU6050_ADDRESS, PWR_MGMT_1_REG, 0x0);
    if(ret) return ret;

    /* Set full scale range of the gyroscope to +-500°/s */
    ret = i2c_write_register(MPU6050_ADDRESS, GYRO_CONFIG_REG, (0x01 << 3));
    if(ret) return ret;

    /* Set full scale range of the accelerometer to +-2g */
    ret = i2c_write_register(MPU6050_ADDRESS, ACCEL_CONFIG_REG, 0x0);
    if(ret) return ret;

    return 0;
}

int get_mpu6050_data(mpu6050_data *data) {

    int ret;

    /* This buffer stores accelerometer and gyroscope data coming from the MPU6050 */
    uint8_t accel_buffer[6], gyro_buffer[6];
    int16_t raw_accel_x, raw_accel_y, raw_accel_z, raw_gyro_x, raw_gyro_y, raw_gyro_z;

    /* Evaluate if data pointer is NULL */
    if (data == NULL) {
        return -1;
    }
    
    ret = i2c_burst_read_registers(MPU6050_ADDRESS, ACCEL_XOUT_H_REG, accel_buffer, 6);
    if(ret) return ret;

    ret = i2c_burst_read_registers(MPU6050_ADDRESS, GYRO_XOUT_H_REG, gyro_buffer, 6);
    if(ret) return ret;

    /* Get 16-bit raw values for each axis */
    raw_accel_x = join_bytes(accel_buffer[0], accel_buffer[1]);
    raw_accel_y = join_bytes(accel_buffer[2], accel_buffer[3]);
    raw_accel_z = join_bytes(accel_buffer[4], accel_buffer[5]);

    raw_gyro_x  = join_bytes(gyro_buffer[0], gyro_buffer[1]);
    raw_gyro_y  = join_bytes(gyro_buffer[2], gyro_buffer[3]);
    raw_gyro_z  = join_bytes(gyro_buffer[4], gyro_buffer[5]);

    /* Store data */
    data->accel_xout = raw_accel_x / accel_lsb_sensitivity;
    data->accel_yout = raw_accel_y / accel_lsb_sensitivity;
    data->accel_zout = raw_accel_z / accel_lsb_sensitivity;

    data->gyro_xout = raw_gyro_x / gyro_lsb_sensitivity;
    data->gyro_yout = raw_gyro_y / gyro_lsb_sensitivity;
    data->gyro_zout = raw_gyro_z / gyro_lsb_sensitivity;

    return 0;
}

