#include "FreeRTOS.h"
#include "ESP8266.h"
#include "stdio.h"
#include "task.h"
#define char *message1="���㿴��������Ѫ����ϼ��ɣ�\r\n"
 

 
static void ESP8266_Task(void* pvParameters);/* ����ʵ�� */

static void ESP8266Init(void);/* ���ڳ�ʼ�����������Դ */





/* 8266������ */
static TaskHandle_t ESP8266_Task_Handle;



/* 8266�����ջ */
static StackType_t ESP8266_Task_Stack[128];
 


/* AppTaskCreate ������ƿ� */
static StaticTask_t ESP8266_Task_TCB;
 
/* ���������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
 
/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;	
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;



void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
																		StackType_t **ppxTimerTaskStackBuffer, 
																		uint32_t *pulTimerTaskStackSize);
 
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
																	 StackType_t **ppxIdleTaskStackBuffer, 
																	 uint32_t *pulIdleTaskStackSize);







int main(void)
{	
	ESP8266Init();
 
	taskENTER_CRITICAL();        
	
	ESP8266_Task_Handle = xTaskCreateStatic(            (TaskFunction_t )ESP8266_Task,		
														 "ESP8266_Task",	
														 128,				
													   	 NULL,			
														 4, 				
														 ESP8266_Task_Stack,	
														 &ESP8266_Task_TCB
										   );	//������ƿ�   
	taskEXIT_CRITICAL();    
	
	if(NULL != ESP8266_Task_Handle)/* �ж��Ƿ񴴽��ɹ� */
	{
		
    vTaskStartScheduler();   /* �������񣬿������� */
		
	}
	
 
	while(1);
	
}





static void ESP8266_Task(void* parameter)//����ʵ����Ϣ���͵ĺ���
{	
   
        WIFI_Connect();
    
}


static void ESP8266Init(void)

{
ESP8266_Init1();
ESP8266_Init2();
//Ҫ�����жϾ�ȥESP8266.C���涨��
}

/**
  **********************************************************************
  * @brief  ��ȡ��������������ջ��������ƿ��ڴ�
	*					ppxTimerTaskTCBBuffer	:		������ƿ��ڴ�
	*					ppxTimerTaskStackBuffer	:	    �����ջ�ڴ�
	*					pulTimerTaskStackSize	:		�����ջ��С
  * @date    2023
  **********************************************************************
  */ 
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* ������ƿ��ڴ� */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* �����ջ�ڴ� */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* �����ջ��С */
}
 
/**
  *********************************************************************
  * @brief  ��ȡ��ʱ������������ջ��������ƿ��ڴ�
	*					ppxTimerTaskTCBBuffer	:		������ƿ��ڴ�
	*					ppxTimerTaskStackBuffer	:	    �����ջ�ڴ�
	*					pulTimerTaskStackSize	:		�����ջ��С
  **********************************************************************
  */ 
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* ������ƿ��ڴ� */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* �����ջ�ڴ� */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* �����ջ��С */
}

