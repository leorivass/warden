#ifndef MPU6050_H
#define MPU6050_H

#define MPU6050_ADDRESS     0x68
#define PWR_MGMT_1_REG      0x6B
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C

#define GYRO_XOUT_H_REG     0x43
#define GYRO_XOUT_L_REG     0x44

#define GYRO_YOUT_H_REG     0x45
#define GYRO_YOUT_L_REG     0x46

#define GYRO_ZOUT_H_REG     0x47
#define GYRO_ZOUT_L_REG     0x48

#define ACCEL_XOUT_H_REG    0x3B
#define ACCEL_XOUT_L_REG    0x3C

#define ACCEL_YOUT_H_REG    0x3D
#define ACCEL_YOUT_L_REG    0x3E

#define ACCEL_ZOUT_H_REG    0x3F
#define ACCEL_ZOUT_L_REG    0x40

typedef struct mpu6050_data {

    float  gyro_xout;
    float  gyro_yout;
    float  gyro_zout;
    
    float accel_xout;
    float accel_yout;
    float accel_zout;

} mpu6050_data;

void mpu6050_init();
int get_mpu6050_data(mpu6050_data*);

#endif