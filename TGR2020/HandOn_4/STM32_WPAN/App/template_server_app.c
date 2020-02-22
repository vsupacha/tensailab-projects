/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/template_server_app.c
 * Description        : P2P Server Application
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN UserCode */
/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "template_server_app.h"
#include "stm32_seq.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint16_t Timestamp;
	uint16_t Axis_X;
	uint16_t Axis_Y;
	uint16_t Axis_Z;
} TEMPLATE_ParameterCharValue_t;

typedef struct
{
  uint8_t  NotificationStatus;
  TEMPLATE_ParameterCharValue_t Parameter;
  int16_t UpdateParameterStep;
  uint8_t UpdateParameter_timer_Id;
} TEMPLATE_Server_App_Context_t;

/* Private defines -----------------------------------------------------------*/
#define PARAMETER_CHANGE_STEP            10
#define PARAMETER_UPDATE_PERIOD          (100*1000/CFG_TS_TICK_VAL) /*100ms*/
#define PARAMETER_VALUE_MAX_THRESHOLD    350
#define PARAMETER_VALUE_MIN_THRESHOLD    100

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
PLACE_IN_SECTION("BLE_APP_CONTEXT") static TEMPLATE_Server_App_Context_t TEMPLATE_Server_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TEMPLATE_UpdateParameter_Timer_Callback(void);
/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void TEMPLATE_APP_context_Init(void);
static void TEMPLATE_Send_Notification_Task(void);

static void TEMPLATE_UpdateParameter_Timer_Callback(void)
{
  UTIL_SEQ_SetTask( 1<<CFG_IdleTask_Update_Temperature, CFG_SCH_PRIO_0);
}

/* Public functions ----------------------------------------------------------*/
void TEMPLATE_STM_App_Notification(TEMPLATE_STM_App_Notification_evt_t *pNotification)
{
  switch(pNotification->Template_Evt_Opcode)
  {
    case TEMPLATE_STM_NOTIFY_ENABLED_EVT:
      TEMPLATE_Server_App_Context.NotificationStatus = 1;
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFICATION ENABLED\n");
      APP_DBG_MSG(" \n\r");
#endif
      /* Start the timer used to update the characteristic */
      HW_TS_Start(TEMPLATE_Server_App_Context.UpdateParameter_timer_Id, PARAMETER_UPDATE_PERIOD);
      break; /* TEMPLATE_STM_NOTIFY_ENABLED_EVT */

    case TEMPLATE_STM_NOTIFY_DISABLED_EVT:
      TEMPLATE_Server_App_Context.NotificationStatus = 0;
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFICATION DISABLED\n");
      APP_DBG_MSG(" \n\r");
#endif
      /* Stop the timer used to update the characteristic */
      HW_TS_Stop(TEMPLATE_Server_App_Context.UpdateParameter_timer_Id);
      break; /* TEMPLATE_STM_NOTIFY_DISABLED_EVT */
      
    case TEMPLATE_STM_WRITE_EVT:
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : WRITE EVENT RECEIVED\n");
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : 0x%x\n",pNotification->DataTransfered.pPayload[0]);
      APP_DBG_MSG(" \n\r");
#endif
      if(pNotification->DataTransfered.pPayload[0] == 0x00)
      {
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : START TASK 2 \n");
#endif
      }
      break; /* TEMPLATE_STM_WRITE_EVT */

#if(BLE_CFG_OTA_REBOOT_CHAR != 0)       
    case TEMPLATE_STM_BOOT_REQUEST_EVT:
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : BOOT REQUESTED\n");
    APP_DBG_MSG(" \n\r");
#endif

      *(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
      NVIC_SystemReset();

      break; /* TEMPLATE_STM_BOOT_REQUEST_EVT */
#endif
      
    default:
      break; /* DEFAULT */
  }

  return;
}

void TEMPLATE_APP_Init(void)
{
	/* Register task used to update the characteristic (send the notification) */
	  UTIL_SEQ_RegTask(1<<CFG_IdleTask_Update_Temperature, UTIL_SEQ_RFU, TEMPLATE_Send_Notification_Task);
  /* Create timer to handle the periodic proprietary temperature sensor data update. */
	HW_TS_Create(CFG_TIM_PROC_ID_ISR,
	  &(TEMPLATE_Server_App_Context.UpdateParameter_timer_Id),
	  hw_ts_Repeated,
	  TEMPLATE_UpdateParameter_Timer_Callback);

  /**
   * Initialize Template application context
   */
  TEMPLATE_Server_App_Context.NotificationStatus=0;
  TEMPLATE_APP_context_Init();
  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void TEMPLATE_APP_context_Init(void)
{
/*	TEMPLATE_Server_App_Context.Parameter.TimeStamp = 0;
	TEMPLATE_Server_App_Context.Parameter.Temperature = 0;*/
	  TEMPLATE_Server_App_Context.Parameter.Axis_X = 10;
	  TEMPLATE_Server_App_Context.Parameter.Axis_Y = 10;
	  TEMPLATE_Server_App_Context.Parameter.Axis_Z = 0;
	  TEMPLATE_Server_App_Context.Parameter.Timestamp = 0;
	TEMPLATE_Server_App_Context.UpdateParameterStep = PARAMETER_CHANGE_STEP;
}

static void TEMPLATE_Send_Notification_Task(void)
{
	uint8_t value[4];
	sensor_t sensor_data

	  value[0] = (uint8_t)(TEMPLATE_Server_App_Context.Parameter.TimeStamp & 0x00FF);
	  value[1] = (uint8_t)(TEMPLATE_Server_App_Context.Parameter.TimeStamp >> 8);

	  read_sensor(&sensor_data)
	  TEMPLATE_Server_App_Context.Parameter.Axis_X = sensor_data.acceleration_x;
	  TEMPLATE_Server_App_Context.Parameter.Axis_Y = sensor_data.acceleration_y;
	  TEMPLATE_Server_App_Context.Parameter.Axis_Z = sensor_data.acceleration_z;

	  TEMPLATE_Server_App_Context.Parameter.Timestamp += PARAMETER_CHANGE_STEP;

	  int16_t accelo[3] = {sensor_data.acceleration_x,sensor_data.acceler ation_y,sensor_data.acceleration_z};
	  int16_t abs_accelo;
	  arm_abs_q15(accelo,&abs_accelo,3);
	  int16_t fall_th = 100;
	  int16_t normal_th = 20;

	  if (abs_accelo < fall_th){
		  if((sensor_data.acceleration_x > fall_th/2 ) || (sensor_data.acceleration_x < -fall_th/2 ) || sensor_data.acceleration_y > fall_th/2 ) || (sensor_data.acceleration_y  < -fall_th/2 ) ){
			  value[2] = 0x00;
			  value[3] = 0xF0;

		  }else {
			  value[2] = 0x00;
			  value[3] = 0x80;
		  }
	  }else if (abs_accelo < normal_th){
		  value[2] = 0x00;
		  value[3] = 0x00;

	  }else {
		  value[2] = 0x00;
		  value[3] = 0x08;
	  }

  if(TEMPLATE_Server_App_Context.NotificationStatus)
  {
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFY CLIENT WITH NEW PARAMETER VALUE \n ");
    APP_DBG_MSG(" \n\r");
#endif
    TEMPLATE_STM_App_Update_Char(0x0000,(uint8_t *)&value);
  }
  else
  {
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : CAN'T INFORM CLIENT - NOTIFICATION DISABLED\n ");
#endif
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/* USER CODE END UserCode */
