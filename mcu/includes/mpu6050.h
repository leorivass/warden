#ifndef MPU6050_H
#define MPU6050_H

#define MPU6050_ADDRESS     0x68
#define PWR_MGMT_1_REG      0x6B
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C


/* 
 *  Data registers in the MPU6050 are arranged sequentially in memory:
 *      0x3B-0x40: Accelerometer (ACCEL_XOUT_H to ACCEL_ZOUT_L)
 *      0x43-0x48: Gyroscope (GYRO_XOUT_H to GYRO_ZOUT_L)
 * 
 *  We only define the starting register for each sensor and use
 *  burst reads to get all consecutive values efficiently.
 */
#define ACCEL_XOUT_H_REG    0x3B
#define GYRO_XOUT_H_REG     0x43

typedef struct mpu6050_data {

    float  gyro_xout;
    float  gyro_yout;
    float  gyro_zout;
    
    float accel_xout;
    float accel_yout;
    float accel_zout;

} mpu6050_data;

int mpu6050_init();
int get_mpu6050_data(mpu6050_data*);

#endif