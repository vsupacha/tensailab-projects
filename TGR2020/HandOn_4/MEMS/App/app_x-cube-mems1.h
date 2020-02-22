/**
  ******************************************************************************
  * File Name          :  stmicroelectronics_x-cube-mems1_7_0_0.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_X_CUBE_MEMS1_H
#define __APP_X_CUBE_MEMS1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks01a2_motion_sensors.h"
#include "iks01a2_motion_sensors_ex.h"
#include "iks01a2_env_sensors.h"
#include "iks01a2_env_sensors_ex.h"

/* Enable sensor masks */
#define PRESSURE_SENSOR                         0x00000001U
#define TEMPERATURE_SENSOR                      0x00000002U
#define HUMIDITY_SENSOR                         0x00000004U
//#define UV_SENSOR                               0x00000008U    /* Reserved for future use */
#define ACCELEROMETER_SENSOR                    0x00000010U
//#define GYROSCOPE_SENSOR                        0x00000020U    /* Reserved for future use */
//#define MAGNETIC_SENSOR                         0x00000040U    /* Reserved for future use */

typedef enum
{
  ACC_INACTIVITY = 0,
  ACC_ACTIVITY = 1
} AccStatusTypeDef;

typedef struct{
  float pressure;          /* in mbar */
  float temperature;       /* in °C   */
  float humidity;          /* in %    */

  float latitude;
  float longitude ;
  float altitudeGps;       /* in m */

  signed short int altitudeBar ;    /* in m * 10 */

  signed int acceleration_x;
  signed int acceleration_y;
  signed int acceleration_z;

  /**more may be added*/
} sensor_t;

/* Exported Functions --------------------------------------------------------*/
void ENV_sensor_Init(void);
void Acc_sensor_init(void);
void sensor_Read( sensor_t *pSensorData);
void MX_MEMS_Init(void);
void MX_MEMS_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_X_CUBE_MEMS1_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
