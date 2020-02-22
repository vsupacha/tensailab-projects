/*
 * User_mems.h
 *
 *  Created on: Dec 31, 2019
 *      Author: panuwatdanklang
 */

#ifndef INC_USER_MEMS_H_
#define INC_USER_MEMS_H_

#define SENSOR_ENABLED


#endif /* INC_USER_MEMS_H_ */

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

void ENV_sensor_Init(void);
void Acc_sensor_init(void);
void sensor_Read( sensor_t *pSensorData);
