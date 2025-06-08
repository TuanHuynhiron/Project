/*
 * mpu6050.h
 *
 *  Created on: Jan 14, 2024
 *      Author: Administrator
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_
#include "main.h"
#define WHO_AM_I_REG 0x75  	//connect
#define PWR_MGMT_1 0x6B    	//weak up
#define SMPLRT_DIV 0x19		//chia tan so
#define MPU6050_ADDR 0xD0  	//id
#define GYRO_CONFIG 0x1B   	//setting goc
#define ACCEL_CONFIG 0x1C	//setting gia toc
#define ACCEL_XOUT_H 0x3B  	//thanh ghi gia toc
#define	GYRO_XOUT_H 0x43	//thanh ghi do
#define Configuration_CONFIG 0x1A
#define	INT_PIN_CFG	0x37
#define	INT_ENABLE 0x38
#define	PWR_MGMT_1 0x6B


#define RAD_TO_DEG 57.295779513082320876798154814105
void mpu_test(I2C_HandleTypeDef *hi2c, uint8_t *test );
uint8_t read_id(I2C_HandleTypeDef *hi2c);
void MPU_inti(I2C_HandleTypeDef *hi2c, uint8_t *check);
void read_xyz(I2C_HandleTypeDef *hi2c,uint8_t *u8_revBuffer,uint8_t id);
void read_GYRO(I2C_HandleTypeDef *hi2c,float *GYRO);
void read_ACCEL(I2C_HandleTypeDef *hi2c,float *ACCEL,double  *x,double *y);
#endif /* INC_MPU6050_H_ */
