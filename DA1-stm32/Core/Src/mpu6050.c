/*
 * mpu6050.c
 *
 *  Created on: Jan 14, 2024
 *      Author: Administrator
 */

#include "math.h"
#include "mpu6050.h"


const double Accel_Z_corrector = 14418.0;
uint8_t read_id(I2C_HandleTypeDef *hi2c)
{
	uint8_t id;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  for (uint8_t i=0; i<255; i++)
  {
	  if (HAL_I2C_IsDeviceReady(hi2c, i,1 ,10)==HAL_OK)
	  {
		  id =i;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	  }
  }
  return id;
}
void mpu_test(I2C_HandleTypeDef *hi2c, uint8_t *test )
{

	HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, PWR_MGMT_1, 1,test, 1, 1000);//danh thuc

	HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, SMPLRT_DIV, 1, test+1, 1, 1000);//8->1Mhz

	HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, GYRO_CONFIG, 1, test+2, 1, 1000);//kich hoach cam bien do

	HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, ACCEL_CONFIG, 1, test+3, 1, 1000);//kich hoach cam bien giatoc
}
void MPU_inti(I2C_HandleTypeDef *hi2c, uint8_t *check)
{
	uint8_t data;
	HAL_I2C_Mem_Read (hi2c, MPU6050_ADDR,WHO_AM_I_REG,1, check, 1, 1000);//check connet
	data = 0x01;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, PWR_MGMT_1, 1,&data, 1, 1000);//danh thuc
	data = 0x05;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, Configuration_CONFIG, 1,&data, 1, 1000);//1khz
	data = 0x32;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, SMPLRT_DIV, 1, &data, 1, 1000);//50hz
	data = 0x08;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, GYRO_CONFIG, 1, &data, 1, 1000);//kich hoach cam bien do
	data = 0x10;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, ACCEL_CONFIG, 1, &data, 1, 1000);//kich hoach cam bien giatoc
	data = 0x80;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, INT_PIN_CFG, 1, &data, 1, 1000);//int len 1 when complet
	data = 0x01;
	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, INT_ENABLE, 1, &data, 1, 1000);//enable int
//	data = 0x01;
//	HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, PWR_MGMT_1, 1, &data, 1, 1000);//on clk

}
void read_ACCEL(I2C_HandleTypeDef *hi2c,float *ACCEL,double *x,double *y)
{
	double Ax,Ay,Az;
	uint8_t Rec_A[6];
	HAL_I2C_Mem_Read (hi2c, MPU6050_ADDR, ACCEL_XOUT_H, 0x01U, Rec_A, 6, 1000);
	Ax = (int16_t)(Rec_A[0] << 8 | Rec_A [1]);
	Ay = (int16_t)(Rec_A[2] << 8 | Rec_A [3]);
	Az = (int16_t)(Rec_A[4] << 8 | Rec_A [5]);
	/* get the float g*/
	*(ACCEL+0) = Ax = (Ax /4096.0 ) ;  //Ax
	*(ACCEL+1) = Ay = (Ay /4096.0 ) ;  //Ay
	*(ACCEL+2) = Az = (Az /4096.0 ) ;  //Az
	double accAngleX, accAngleY;
	accAngleX = (atan(Ay / sqrt(pow(Ax, 2) + pow(Az, 2))) * 180 /(atan(1)*4)) ; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
	accAngleY = (atan(-1 * Ax / sqrt(pow(Ay, 2) + pow(Az, 2))) * 180 / (atan(1)*4)) ; // AccErrorY ~(-1.58)
//	double roll,pitch;
//	pitch = atan2(Ax,sqrt(pow(Ay,2)+pow(Az,2)))*180/(atan(1)*4);
//	roll =atan2(-Ay,Az)*180/(atan(1)*4);
	*x=accAngleX;
	*y=accAngleY;
}

void read_GYRO(I2C_HandleTypeDef *hi2c,float *GYRO)
{
	// Read 6 BYTES of data starting from GYRO_XOUT_H register
	uint8_t Gyro_X_RAW,Gyro_Y_RAW, Gyro_Z_RAW;
	uint8_t Rec_G[6];
	HAL_I2C_Mem_Read (hi2c, MPU6050_ADDR, GYRO_XOUT_H, 0x01U, Rec_G, 6, 1000);

	Gyro_X_RAW = (int16_t)(Rec_G[0] << 8 | Rec_G [1]);
	Gyro_Y_RAW = (int16_t)(Rec_G[2] << 8 | Rec_G [3]);
	Gyro_Z_RAW = (int16_t)(Rec_G[4] << 8 | Rec_G [5]);

	/*** convert the RAW values into dps (°/s)
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 131.0
	     for more details check GYRO_CONFIG Register              ****/

	*(GYRO+0) = (Gyro_X_RAW/131.0)*RAD_TO_DEG;
	*(GYRO+1) = (Gyro_Y_RAW/131.0)*RAD_TO_DEG;
	*(GYRO+2) = (Gyro_Z_RAW/131.0)*RAD_TO_DEG;

}

void read_xyz(I2C_HandleTypeDef *hi2c,uint8_t *u8_revBuffer,uint8_t id)
{
		HAL_I2C_Mem_Read_IT(hi2c,id,0x00, 0x01U, u8_revBuffer, 4);
}
