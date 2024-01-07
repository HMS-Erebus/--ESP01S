#include "FreeRTOS.h"
#include "ESP8266.h"
#include "stdio.h"
#include "task.h"
#define char *message1="让你看看我们热血的组合技吧！\r\n"
 

 
static void ESP8266_Task(void* pvParameters);/* 任务实现 */

static void ESP8266Init(void);/* 用于初始化板载相关资源 */





/* 8266任务句柄 */
static TaskHandle_t ESP8266_Task_Handle;



/* 8266任务堆栈 */
static StackType_t ESP8266_Task_Stack[128];
 


/* AppTaskCreate 任务控制块 */
static StaticTask_t ESP8266_Task_TCB;
 
/* 空闲任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
 
/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;	
/* 定时器任务控制块 */
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
										   );	//任务控制块   
	taskEXIT_CRITICAL();    
	
	if(NULL != ESP8266_Task_Handle)/* 判断是否创建成功 */
	{
		
    vTaskStartScheduler();   /* 启动任务，开启调度 */
		
	}
	
 
	while(1);
	
}





static void ESP8266_Task(void* parameter)//我们实现信息发送的函数
{	
   
        WIFI_Connect();
    
}


static void ESP8266Init(void)

{
ESP8266_Init1();
ESP8266_Init2();
//要定义中断就去ESP8266.C里面定义
}

/**
  **********************************************************************
  * @brief  获取空闲任务的任务堆栈和任务控制块内存
	*					ppxTimerTaskTCBBuffer	:		任务控制块内存
	*					ppxTimerTaskStackBuffer	:	    任务堆栈内存
	*					pulTimerTaskStackSize	:		任务堆栈大小
  * @date    2023
  **********************************************************************
  */ 
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* 任务控制块内存 */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* 任务堆栈内存 */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* 任务堆栈大小 */
}
 
/**
  *********************************************************************
  * @brief  获取定时器任务的任务堆栈和任务控制块内存
	*					ppxTimerTaskTCBBuffer	:		任务控制块内存
	*					ppxTimerTaskStackBuffer	:	    任务堆栈内存
	*					pulTimerTaskStackSize	:		任务堆栈大小
  **********************************************************************
  */ 
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* 任务控制块内存 */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* 任务堆栈内存 */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* 任务堆栈大小 */
}

