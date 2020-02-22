/*
 * User_mems.c
 *
 *  Created on: Dec 31, 2019
 *      Author: panuwatdanklang
 */

#include "User_mems.h"
#include "main.h"
#include "iks01a2_motion_sensors.h"
#include "iks01a2_motion_sensors_ex.h"
#include "iks01a2_env_sensors.h"
#include "iks01a2_env_sensors_ex.h"

void *LSM303AGR_X_handle = 0; /* Needed for testing LSM303AGR onboard presence. */

extern volatile uint32_t SensorsEnabled; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
volatile uint32_t SensorsEnabled = 0;

volatile uint16_t mems_int1_detected;
volatile uint16_t mems_int2_detected;
extern uint16_t bUserButtonPushed;
AccStatusTypeDef accActInact;



void ENV_sensor_Init(void){
	  /* USER CODE BEGIN 6 */
	  /* Initialize sensors */
	  if ( IKS01A2_ENV_SENSOR_Init(IKS01A2_HTS221_0, ENV_TEMPERATURE | ENV_HUMIDITY) == BSP_ERROR_NONE )
	  {
	    //PRINTF("Humidity & Temperature sensor initialized\r\n");
	    SensorsEnabled |= TEMPERATURE_SENSOR;
	    SensorsEnabled |= HUMIDITY_SENSOR;
	  }
	  if ( IKS01A2_ENV_SENSOR_Init(IKS01A2_LPS22HB_0, ENV_PRESSURE) == BSP_ERROR_NONE )
	  {
	    //PRINTF("Pressure sensor initialized\r\n");
	    SensorsEnabled |= PRESSURE_SENSOR;
	  }
	  /* USER CODE BEGIN 6 */
}
void Acc_sensor_init(void){
	/* USER CODE BEGIN 7 */
/* --- Initialization --- */
if ( IKS01A2_MOTION_SENSOR_Init(IKS01A2_LSM6DSL_0, MOTION_ACCELERO ) == BSP_ERROR_NONE )
{
  //if (!quiet) PRINTF("Accelerometer initialized\r\n");
}
else
{
  //PRINTF("Accelerometer NOT initialized\r\n");
  Error_Handler();
}

/* --- Enabling --- */
if ( IKS01A2_MOTION_SENSOR_Enable(IKS01A2_LSM6DSL_0, MOTION_ACCELERO) == BSP_ERROR_NONE )
{
  SensorsEnabled |= ACCELEROMETER_SENSOR;
  //if (!quiet) PRINTF("Accelerometer enabled\r\n");
}
else
{
  //PRINTF("Accelerometer NOT enabled\r\n");
  Error_Handler();
}
/* USER CODE BEGIN 7 */
}

void sensor_Read( sensor_t *pSensorData)
{
  /* USER CODE BEGIN 5 */
  float HUMIDITY_Value = 0;
  float TEMPERATURE_Value = 0;
  float PRESSURE_Value = 0;

  IKS01A2_MOTION_SENSOR_Axes_t AccValue;
  uint8_t status = 0;

  if ( SensorsEnabled & HUMIDITY_SENSOR )
    IKS01A2_ENV_SENSOR_GetValue(IKS01A2_HTS221_0, ENV_HUMIDITY, &HUMIDITY_Value);
  if ( SensorsEnabled & TEMPERATURE_SENSOR )
    IKS01A2_ENV_SENSOR_GetValue(IKS01A2_HTS221_0, ENV_TEMPERATURE, &TEMPERATURE_Value);
  if ( SensorsEnabled & PRESSURE_SENSOR )
    IKS01A2_ENV_SENSOR_GetValue(IKS01A2_LPS22HB_0, ENV_PRESSURE, &PRESSURE_Value);

 // if (IKS01A2_MOTION_SENSOR_Get_DRDY_Status(IKS01A2_LSM6DSL_0, MOTION_ACCELERO, &status) == BSP_ERROR_NONE && status == 1U)
    IKS01A2_MOTION_SENSOR_GetAxes( IKS01A2_LSM6DSL_0, MOTION_ACCELERO, &AccValue );

  pSensorData->humidity    = HUMIDITY_Value;
  pSensorData->temperature = TEMPERATURE_Value;
  pSensorData->pressure    = PRESSURE_Value;

  pSensorData->acceleration_x = AccValue.x;
  pSensorData->acceleration_y = AccValue.y;
  pSensorData->acceleration_z = AccValue.z;

  /* USER CODE END 5 */
}
