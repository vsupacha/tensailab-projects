/**
  ******************************************************************************
  * File Name          :  stmicroelectronics_x-cube-mems1_7_0_0.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.7.0.0 instances.
  ******************************************************************************
  *
  * COPYRIGHT 2020 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ******************************************************************************
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_x-cube-mems1.h"
#include "main.h"
#include <stdio.h>
#include "iks01a2_motion_sensors.h"
#include "iks01a2_motion_sensors_ex.h"
#include "iks01a2_env_sensors.h"
#include "iks01a2_env_sensors_ex.h"


extern volatile uint32_t SensorsEnabled; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
volatile uint32_t SensorsEnabled = 0;

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

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */ 
//	ENV_sensor_Init();
//	Acc_sensor_init();
  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */
  
  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  /* USER CODE BEGIN MEMS_Init_PostTreatment */
  
  /* USER CODE END MEMS_Init_PostTreatment */
}
/*
 * LM background task
 */
void MX_MEMS_Process(void)
{
  /* USER CODE BEGIN MEMS_Process_PreTreatment */
  
  /* USER CODE END MEMS_Process_PreTreatment */

  /* USER CODE BEGIN MEMS_Process_PostTreatment */
  
  /* USER CODE END MEMS_Process_PostTreatment */
}


#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
