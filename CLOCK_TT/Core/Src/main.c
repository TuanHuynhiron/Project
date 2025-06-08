/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <clock_TT-h/fatfs_sd.h>
#include <clock_TT-h/R-W-SD.h>
#include <clock_TT-h/ssd1306.h>
#include <clock_TT-h/ssd1306_tests.h>
#include <clock_TT-h/RTCds3231.h>
#include <clock_TT-h/DHT11.h>
#include <clock_TT-h/manhinh.h>
#include "FLASH.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

//bien man hinh chinh--------
const unsigned char battery []= {
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x7f, 0xff, 0xf8,
	0xe0, 0x00, 0x1c,
	0xc0, 0x00, 0x0c,
	0xdf, 0xff, 0xee,
	0xdf, 0xff, 0xef,
	0xdf, 0xff, 0xe3,
	0xdf, 0xff, 0xe3,
	0xdf, 0xff, 0xe3,
	0xdf, 0xff,	0xe3,
	0xdf, 0xff, 0xef,
	0xdf, 0xff, 0xee,
	0xc0, 0x00, 0x0c,
	0xe0, 0x00, 0x1c,
	0x7f, 0xff, 0xf8,
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00};
DS3231_typedef DS3231_TimeNow;
DS3231_typedef DS3231_TimeSet;
uint8_t u8_revBuffer[7];
char giophut[5],giay[4],ngay[20];
char giophutgiay[30]="00:00:00                  ";
char ngays[30]="thu0,00-00-2000        ";
char giophut_a[30]="00:00             ";
uint8_t timeset[7];
uint8_t timeset2[7];
uint8_t timealrm[2];
uint32_t min;
uint8_t date;
uint8_t id;
uint8_t set=0;
uint8_t tat=0;
//bien mang hinh chinh---------
volatile uint8_t mode=0;
uint8_t controdoc=0;
uint8_t dung=0;
uint8_t controngang=0;
volatile uint8_t 	ok=0;
volatile uint8_t	trai=0;
volatile uint8_t	phai=0;
volatile uint8_t	tren=0;
volatile uint8_t    duoi=0;
char ghi[5];
//bien phat nhac-------
char buffer[1024+1];
uint32_t cnt=1;
uint32_t ngat=0;
char phat[20]="1.txt";
uint8_t amthanh[1024/2];
char nhac[80];
//bien phat nhac-------
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  {
    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
             the HAL_GPIO_EXTI_Callback could be implemented in the user file
     */
    min=HAL_GetTick();
    if (GPIO_Pin== GPIO_PIN_1)
  	      {
    	if (mode==4 && controdoc==1) mode=0;
    	if (mode!=0 && controdoc!=0) ok=1;
    	else mode=0;
  	      }
  	else if(GPIO_Pin == GPIO_PIN_2)
  	      {
  		if (mode==1 && controdoc!=0) trai=1;
  		else if (mode==4 && controdoc!=0) trai=1;
  		else mode=1;
  	      }
  	else if(GPIO_Pin == GPIO_PIN_3)
  	      {
  		if (mode==1 && controdoc!=0) phai=1;
  		else if (mode==4 && controdoc!=0) phai=1;
  		else mode=2;
  	      }
  	else if(GPIO_Pin == GPIO_PIN_5){
  		tren=1;
  		dung=1;
  	}
  	else if(GPIO_Pin == GPIO_PIN_4){
  		duoi=1;
  		dung=1;
  	  	}
//    while(HAL_GetTick()-min<20);

  }
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //16Khz timer
  	{

  	   TIM2->CCR1=amthanh[cnt++];
  	   if(cnt>=512) {
  		 cnt=0;
  		HAL_TIM_Base_Stop_IT(&htim3);
  		ngat=1;
  	   }
  	}
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{

		  UNUSED(hi2c);
		if (hi2c->Instance == I2C1)
		{
			DS3231_TimeNow.sec=BCD2DEC(u8_revBuffer[0]);
			DS3231_TimeNow.min=BCD2DEC(u8_revBuffer[1]);
			DS3231_TimeNow.hours=BCD2DEC(u8_revBuffer[2]);
			DS3231_TimeNow.date=BCD2DEC(u8_revBuffer[3]);
			DS3231_TimeNow.day=BCD2DEC(u8_revBuffer[4]);
			DS3231_TimeNow.month=BCD2DEC(u8_revBuffer[5]);
			DS3231_TimeNow.year=BCD2DEC(u8_revBuffer[6]);
		}

}
uint8_t doc(uint8_t *tren,uint8_t *duoi)
{
	static uint8_t i=0;
	if (*tren==1) {
		i--;
		*tren=0;
	}
	if (*duoi==1) {
		i++;
		*duoi=0;
	}
	if(set==1) i=i;
	else{
		if (i<0 || i>4) i=0;
	}
	return i;
}
uint8_t ngang(uint8_t *trai,uint8_t *phai)
{
	static uint8_t i=0;
	if (*trai==1) {
		i--;
		*trai=0;
	}
	if (*phai==1) {
		i++;
		*phai=0;
	}
	if(i<0 || i>17) i=0;
	return i;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1); //cho phep PWM (gia lap DAC)
  ssd1306_Init();
////  man hinh chinh begin
  //doc
giay[0]=':';
giophut[2]=':';
ngay[0]='T';ngay[1]='h';ngay[2]='u';ngay[4]=',';
ngay[7]='-';
ngay[10]='-';
ngay[11]='2';ngay[12]='0';

id=stactusTX();
   // manh hinh chinh end
   //phat nhac begin
  //phat nhac end
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	switch (mode){
	 case 0:
	 	 {
		 while(mode==0){//man hinh chinh
			 dung=0;
			 readsdtxt("","tb.txt",ghi,5);
			 str2uint8t(ghi,timealrm,2);
			 readtime(u8_revBuffer,id);
			 if (timealrm[1]==(DS3231_TimeNow.hours) && timealrm[0]==(DS3231_TimeNow.min))
			 	 {

				 	if (tat==0)readline("","3.txt",buffer,amthanh,1024/2,&ngat,&dung);
				 	tat=1;
			 	 }

			 ngay[3]=(DS3231_TimeNow.date) + 48;
			 ngay[5]=((DS3231_TimeNow.day)/10) + 48;
			 ngay[6]=((DS3231_TimeNow.day)%10) + 48;
			 ngay[8]=((DS3231_TimeNow.month)/10) + 48;
			 ngay[9]=((DS3231_TimeNow.month)%10) + 48;
			 ngay[13]=((DS3231_TimeNow.year)/10) + 48;
			 ngay[14]=((DS3231_TimeNow.year)%10)  + 48;
			 giay[1]=((DS3231_TimeNow.sec)/10) + 48;
			 giay[2]=((DS3231_TimeNow.sec)%10) + 48;
			 giophut[0]=((DS3231_TimeNow.hours)/10) +48;
			 giophut[1]=((DS3231_TimeNow.hours)%10) +48;
			 giophut[3]=((DS3231_TimeNow.min)/10) +48;
			 giophut[4]=((DS3231_TimeNow.min)%10) +48;
			 ssd1306_SetCursor(10,0);
			 ssd1306_WriteString("Clock TT",Font_11x18,White);
			 ssd1306_DrawBitmap(101,0,battery,24,18,White);
			 ssd1306_UpdateScreen();
			 ssd1306_SetCursor(0,18);
			 ssd1306_WriteString(ngay,Font_7x10,White);
			 ssd1306_UpdateScreen();
			 ssd1306_SetCursor(37,28);
			 ssd1306_WriteString(giophut,Font_11x18,White);
			 ssd1306_SetCursor(90,35);
			 ssd1306_WriteString(giay,Font_7x10,White);
			 ssd1306_UpdateScreen();
	 	 	 }
		 ssd1306_Init();
		 break;
	 	 }//case 0
	 case 1:
	 	 {
		 while(mode ==1){// man hinh cai dat
			 ngays[3]=(timeset[3]) + 48;
			 ngays[5]=((timeset[4])/10) + 48;
			 ngays[6]=((timeset[4])%10) + 48;
			 ngays[8]=((timeset[5])/10) + 48;
			 ngays[9]=((timeset[5])%10) + 48;
			 ngays[13]=((timeset[6])/10) + 48;
			 ngays[14]=((timeset[6])%10)  + 48;

			 giophutgiay[0]=((timeset[2])/10) +48;//gio
			 giophutgiay[1]=((timeset[2])%10) +48;
			 giophutgiay[3]=((timeset[1])/10) +48;//phut
			 giophutgiay[4]=((timeset[1])%10) +48;
			 giophutgiay[6]=((timeset[0])/10) +48;//giay
			 giophutgiay[7]=((timeset[0])%10) +48;

			 controdoc=doc(&tren,&duoi);
			 controngang=ngang(&trai,&phai);
			 ssd1306_SetCursor(0,0);
			 ssd1306_WriteString("Time Set",Font_11x18,White);
			 ssd1306_UpdateScreen();
			 ssd1306_SetCursor(0,18);
			 ssd1306_WriteString(ngays,Font_7x10,White);
			 ssd1306_SetCursor(0,28);
			 ssd1306_WriteString(giophutgiay,Font_7x10,White);
			 ssd1306_UpdateScreen();
			 ssd1306_SetCursor(0,38);
			 ssd1306_WriteString("                 ",Font_7x10,White);
			 ssd1306_UpdateScreen();
			 ssd1306_SetCursor(0,48);
			 ssd1306_WriteString("Bao thuc          ",Font_7x10,White);
			 ssd1306_UpdateScreen();
			 ssd1306_SetCursor(0+controngang*7,8+controdoc*10);
			 ssd1306_WriteString(" ",Font_7x10,Black);
			 ssd1306_UpdateScreen();
			 if (ok==1){
				 ok=0;
				 set=1;
				 while (ok==0){

					 if (controdoc==1 && controngang==3)  {
						 timeset[3]=doc(&tren,&duoi);	//thu
						 if (timeset[3]>8) timeset[3]=2;
						 if (timeset[3]<2) timeset[3]=8;
						 ngays[3]=(timeset[3]) + 48;
					 }
					 if (controdoc==1 && controngang==6) {
						 timeset[4]=doc(&tren,&duoi);//ngay
						 if (timeset[4]>31) timeset[4]=0;
						 if (timeset[4]<1) timeset[4]=31;
						 ngays[5]=((timeset[4])/10) + 48;
						 ngays[6]=((timeset[4])%10) + 48;
					 }
					 if (controdoc==1 && controngang==9) {
						 timeset[5]=doc(&tren,&duoi);//thang
						 if (timeset[5]>12) timeset[5]=1;
						 if (timeset[5]<1) timeset[5]=12;
						 ngays[8]=((timeset[5])/10) + 48;
						 ngays[9]=((timeset[5])%10) + 48;

					 }
					 if (controdoc==1 && controngang==14) {
						 timeset[6]=doc(&tren,&duoi);//nam
						 if (timeset[6]>99) timeset[6]=99;
						 if (timeset[6]<0) timeset[6]=0;
						 ngays[13]=((timeset[6])/10) + 48;
						 ngays[14]=((timeset[6])%10)  + 48;

					 }
					 if (controdoc==2 && controngang==1) {
						 timeset[2]=doc(&tren,&duoi);//gio
						 if (timeset[2]>23) timeset[2]=0;
						 if (timeset[2]<0) timeset[2]=23;
						 giophutgiay[0]=((timeset[2])/10) +48;//gio
						 giophutgiay[1]=((timeset[2])%10) +48;
					 }
					 if (controdoc==2 && controngang==4) {
						 timeset[1]=doc(&tren,&duoi);//phut
						 if (timeset[1]>59) timeset[1]=0;
						 if (timeset[1]<0) timeset[1]=59;
						giophutgiay[3]=((timeset[1])/10) +48;//phut
						giophutgiay[4]=((timeset[1])%10) +48;
					 }
					 if (controdoc==2 && controngang==7) {
						 timeset[0]=doc(&tren,&duoi);//giay
						 if (timeset[0]>59) timeset[0]=0;
						 if (timeset[0]<0)  timeset[0]=59;
						 giophutgiay[6]=((timeset[0])/10) +48;//giay
						 giophutgiay[7]=((timeset[0])%10) +48;
					 }
					 if (controdoc==4 && controngang==0){
					 mode=4;
					 }
					  	 	 	 ssd1306_SetCursor(0,18);
					 	 	 	 ssd1306_WriteString(ngays,Font_7x10,White);
					 			 ssd1306_SetCursor(0,28);
					 			 ssd1306_WriteString(giophutgiay,Font_7x10,White);
					 			 ssd1306_UpdateScreen();
					 			 ssd1306_SetCursor(0,38);
					 			 ssd1306_WriteString("                 ",Font_7x10,White);
					 			 ssd1306_UpdateScreen();
					 			 ssd1306_SetCursor(0,48);
					 			 ssd1306_WriteString("Setting time          ",Font_7x10,White);
					 			 ssd1306_UpdateScreen();
				 }
				 controdoc = 1;
				 for (int i=0;i<7;i++){
					 timeset2[i]=timeset[i];
				 }
				 if (mode!=4) settime(timeset2,id);
				 ok=0;
				 set=0;

			 }

		 }
		 ssd1306_Init();
		 break;
	 	 }//case1
	 case 2:
	 	 {
		  while (mode ==2){// man hinh phat nhac
			 ssd1306_SetCursor(0,0);
			 ssd1306_WriteString("Music         ",Font_11x18,White);
			 ssd1306_UpdateScreen();
			 readdanhsachnhac("","dsn.txt",nhac);
			 controdoc=doc(&tren,&duoi);
			 phat[0]=controdoc+48;
			 ssd1306_SetCursor(0,8+controdoc*10);
			 ssd1306_WriteString(">",Font_7x10,White);
			 ssd1306_UpdateScreen();
		if(ok==1) {
			ok=0;
			readline("",phat,buffer,amthanh,1024/2,&ngat,&dung);
			}

		 }
		 ssd1306_Init();
		 break;
	 	 }//case2
	 case 4:
		 {
			 while (mode==4){
				 tat=0;
				 giophut_a[0]=((timealrm[1])/10) +48;//gio
				 giophut_a[1]=((timealrm[1])%10) +48;
				 giophut_a[3]=((timealrm[0])/10) +48;//phut
				 giophut_a[4]=((timealrm[0])%10) +48;


				 controdoc=doc(&tren,&duoi);
				 controngang=ngang(&trai,&phai);
				 ssd1306_SetCursor(0,0);
				 ssd1306_WriteString("Time Set",Font_11x18,White);
				 ssd1306_UpdateScreen();
				 ssd1306_SetCursor(0,18);
				 ssd1306_WriteString("X.Man hinh chinh",Font_7x10,White);
				 ssd1306_SetCursor(0,28);
				 ssd1306_WriteString(giophut_a,Font_7x10,White);
				 ssd1306_UpdateScreen();
				 ssd1306_SetCursor(0,38);
				 ssd1306_WriteString("----------------",Font_7x10,White);
				 ssd1306_UpdateScreen();
				 ssd1306_SetCursor(0,48);
				 ssd1306_WriteString("----------------",Font_7x10,White);
				 ssd1306_UpdateScreen();
				 ssd1306_SetCursor(0+controngang*7,8+controdoc*10);
				 ssd1306_WriteString(" ",Font_7x10,Black);
				 ssd1306_UpdateScreen();
				 if (ok==1){
					 ok=0;
					 set=1;
					 while (ok==0){
						 if (controdoc==2 && controngang==1) {
							 timealrm[1]=doc(&tren,&duoi);//gio
							 if (timealrm[1]>23) timealrm[1]=0;
							 if (timealrm[1]<0) timealrm[1]=23;
							 giophut_a[0]=((timealrm[1])/10) +48;//gio
							 giophut_a[1]=((timealrm[1])%10) +48;
						 }
						 if (controdoc==2 && controngang==4) {
							 timealrm[0]=doc(&tren,&duoi);//phut
							 if (timealrm[0]>59) timealrm[0]=0;
							 if (timealrm[0]<0) timealrm[0]=59;
							 giophut_a[3]=((timealrm[0])/10) +48;//phut
							 giophut_a[4]=((timealrm[0])%10) +48;
						 }
						 if (mode==0 && controdoc==1){
							 set=0;
							 tat=0;
							 write_sd("","tb.txt",ghi,timealrm,2);
							 break;
						 }
						 ssd1306_SetCursor(0,0);
						 ssd1306_WriteString("Time Set",Font_11x18,White);
						 ssd1306_UpdateScreen();
						 ssd1306_SetCursor(0,18);
						 ssd1306_WriteString("X-Man hinh chinh",Font_7x10,White);
						 ssd1306_SetCursor(0,28);
						 ssd1306_WriteString(giophut_a,Font_7x10,White);
						 ssd1306_UpdateScreen();
						 ssd1306_SetCursor(0,38);
						 ssd1306_WriteString("Dang cai dat     ",Font_7x10,White);
						 ssd1306_UpdateScreen();
						 ssd1306_SetCursor(0,48);
						 ssd1306_WriteString("                 ",Font_7x10,White);
						 ssd1306_UpdateScreen();
					 }//while
					 tat=0;
					 write_sd("","tb.txt",ghi,timealrm,2);
					 ok=0;
					 set=0;
					 controdoc = 1;
				 }
			 }
			 ssd1306_Init();
			 break;
		 	 }//case 4
	 	 }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 255;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 36;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 125;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 71;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0xffff;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, cs_Pin|GPIO_PIN_8|DHT11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, dc_Pin|rs_Pin|SD_CARD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : OK_Pin TRAI_Pin PHAI_Pin */
  GPIO_InitStruct.Pin = OK_Pin|TRAI_Pin|PHAI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : cs_Pin */
  GPIO_InitStruct.Pin = cs_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(cs_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : dc_Pin rs_Pin */
  GPIO_InitStruct.Pin = dc_Pin|rs_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CARD_Pin */
  GPIO_InitStruct.Pin = SD_CARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_CARD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 DHT11_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_8|DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DUOI_Pin TREN_Pin */
  GPIO_InitStruct.Pin = DUOI_Pin|TREN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
