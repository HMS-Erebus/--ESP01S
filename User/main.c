/*FreeRTOS相关头文件*/
#include "freertos.h"
#include "task.h"
#include "timers.h"
/********************/
#include "stm32f10x.h"                // Device header
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "max30102.h"
#include "algorithm.h"
#include "xiic.h"
#include "blood.h"


/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* esp8266任务句柄 */
static TaskHandle_t esp8266_Task_Handle = NULL;
/* max30102任务句柄 */
static TaskHandle_t max30102_Task_Handle = NULL;
/* 蜂鸣器任务句柄 */
static TaskHandle_t buzzer_Task_Handle = NULL;
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void esp8266_Task(void* pvParameters);/* esp8266_Task任务实现 */
static void max30102_Task(void* pvParameters);/* max30102_Task任务实现 */
static void buzzer_Task(void* pvParameters);/* buzzer_Task任务实现 */
//static void BSP_Init(void);/* 用于初始化板载相关资源 */
/*
*************主函数声明***************
*/
int main(void)
{	
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    
  /* 开发板硬件初始化 */
  //BSP_Init();
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);  /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建esp8266_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )esp8266_Task, /* 任务入口函数 */
                        (const char*    )"esp8266_Task",/* 任务名字 */
                        (uint16_t       )64,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&esp8266_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
  
	/* 创建max30102_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )max30102_Task, /* 任务入口函数 */
                        (const char*    )"max30102_Task",/* 任务名字 */
                        (uint16_t       )64,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&max30102_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
	/* 创建buzzer_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )buzzer_Task, /* 任务入口函数 */
                        (const char*    )"buzzer_Task",/* 任务名字 */
                        (uint16_t       )64,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )4,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&buzzer_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)

	
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}
/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/



static void esp8266_Task(void* parameter)
{	
	UsartPrintf(USART_DEBUG," AT+RST\r\n");
	vTaskDelay(5000);
	
	UsartPrintf(USART_DEBUG,"AT+CWMODE=1\r\n");
	vTaskDelay(5000);
	
	UsartPrintf(USART_DEBUG,"AT+CWJAP=\"superli\",\"08080808\"\r\n");
	vTaskDelay(8000);
	
	UsartPrintf(USART_DEBUG,"AT+CIPSNTPCFG=1,8,\"ntpl.aliyun.com\"\r\n");
	vTaskDelay(8000);
	
	UsartPrintf(USART_DEBUG,"AT+MQTTUSERCFG=0,1,\"NULL\",\"MAX30102&k0qbh9ZPUor\",\"AD810A0D4D9A77CB906AC575E20EF3B0F5854370\",0,0,\"\"\r\n");
	vTaskDelay(5000);
	//"AT+MQTTUSERCFG=0,1,"NULL",\"DEVICE1&k0qgo9UYc52\",\"9f966f81f976d62ad9898c499dcdc1bd7d360272ca05c2b19991dcb38baeb249\",0,0,\"\"\r\n"
	UsartPrintf(USART_DEBUG,"AT+MQTTCLIENTID=0,\"002211|securemode=3\\,signmethod=hmacsha1\\,timestamp=001122|\"\r\n");
	vTaskDelay(5000);
	//"AT+MQTTCLIENTID=0,\"k0qgo9UYc52.DEVICE1|securemode=2\\,signmethod=hmacsha256\\,timestamp=1704763694976|\"\r\n"
	UsartPrintf(USART_DEBUG,"AT+MQTTCONN=0,\"k0qbh9ZPUor.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n");
	vTaskDelay(5000);
	//"AT+MQTTCONN=0,\"iot-06z00ialvl2dkyw.mqtt.iothub.aliyuncs.com\",1883,1\r\n"
	UsartPrintf(USART_DEBUG,"AT+MQTTSUB=0,\"/sys/k0qbh9ZPUor/esp8266/thing/service/property/set\",1\r\n");
	vTaskDelay(5000);
	//"AT+MQTTSUB=0,\"/sys/k0qgo9UYc52/DEVICE1/thing/event/property/post\",1\r\n"
	//订阅设备属性上报
	UsartPrintf(USART_DEBUG,"AT+MQTTSUB=0,\"/k0qbh9ZPUor/esp8266/user/get\",1\r\n");
	vTaskDelay(5000);
    while (1);
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void max30102_Task(void* parameter)
{	
	extern float Heart_Rate;
	extern float sp02_num;
	int i=0;
	//Delay_init(72);             //设置外部晶振和单片机时钟频率为72m
	OLED_Init();				  //OLED初始化
	OLED_ShowString(1,1,"hello");
	Usart1_Init(115200);		  //调试串口初始化
	Usart2_Init(115200);		  //串口初始
	
	IIC_GPIO_INIT();		
	MAX30102_GPIO();
	Max30102_reset();
	MAX30102_Config();
	for(i = 0;i < 128;i++) 
	{
		while(MAX30102_INTPin_Read()==0)
		{
			//读取FIFO
			max30102_read_fifo();
		}
	}
    while (1)
    {
		 blood_Loop();
		
    }
}
static void buzzer_Task(void* parameter)
{
	
}
//int main(void)
//{
//	int i=0;
//	OLED_Init();				//OLED初始化
//	OLED_ShowString(1,1,"hello");
//	Usart1_Init(115200);		//调试串口初始化
//	Usart2_Init(115200);		//串口初始
//	
//	UsartPrintf(USART_DEBUG," AT+RST\r\n");
//	Delay_ms(5000);
//	
//	UsartPrintf(USART_DEBUG,"AT+CWMODE=1\r\n");
//	Delay_ms(5000);
//	
//	UsartPrintf(USART_DEBUG,"AT+CWJAP=\"superli\",\"08080808\"\r\n");
//	Delay_ms(8000);
//	
//	UsartPrintf(USART_DEBUG,"AT+CIPSNTPCFG=1,8,\"ntpl.aliyun.com\"\r\n");
//	Delay_ms(8000);
//	
//	UsartPrintf(USART_DEBUG,"AT+MQTTUSERCFG=0,1,\"NULL\",\"MAX30102&k0qbh9ZPUor\",\"AD810A0D4D9A77CB906AC575E20EF3B0F5854370\",0,0,\"\"\r\n");
//	Delay_ms(5000);
//	UsartPrintf(USART_DEBUG,"AT+MQTTCLIENTID=0,\"002211|securemode=3\\,signmethod=hmacsha1\\,timestamp=001122|\"\r\n");
//	Delay_ms(5000);
//	UsartPrintf(USART_DEBUG,"AT+MQTTCONN=0,\"k0qbh9ZPUor.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n");
//	Delay_ms(5000);
//	UsartPrintf(USART_DEBUG,"AT+MQTTSUB=0,\"/sys/k0qbh9ZPUor/esp8266/thing/service/property/set\",1\r\n");
//	Delay_ms(5000);
//	UsartPrintf(USART_DEBUG,"AT+MQTTSUB=0,\"/k0qbh9ZPUor/esp8266/user/get\",1\r\n");
//	IIC_GPIO_INIT();		
//	MAX30102_GPIO();
//	Max30102_reset();
//	MAX30102_Config();
//	
//	
//	  
//	for(i = 0;i < 128;i++) 
//	{
//		while(MAX30102_INTPin_Read()==0)
//		{
//			//读取FIFO
//			max30102_read_fifo();
//		}
//	}
//	while (1)
//	{
//		//blood_Loop();				//获取血氧和心率
//		//Delay_ms(1000);
////		UsartPrintf(USART_DEBUG,"AT+MQTTPUB=0,\"/k0qbh9ZPUor/esp8266/user/update\",\"%d\",1,0\r\n",temperature);
////		Delay_ms(3000);
////		UsartPrintf(USART_DEBUG,"AT+MQTTPUB=0,\"/k0qbh9ZPUor/esp8266/user/update\",\"%d\",1,0\r\n",humidity);
////		Delay_ms(3000);
////		UsartPrintf(USART_DEBUG,"AT+MQTTPUB=0,\"/sys/k0qbh9ZPUor/esp8266/thing/event/property/post\",\"{\\\"params\\\":{\\\"blood\\\":%d\\}}\",1,0\r\n",temperature);
////		Delay_ms(3000);
////		UsartPrintf(USART_DEBUG,"AT+MQTTPUB=0,\"/sys/k0qbh9ZPUor/esp8266/thing/event/property/post\",\"{\\\"params\\\":{\\\"HeartRate\\\":%d\\}}\",1,0\r\n",humidity);
////	
//	}
//}
//static void BSP_Init(void)
//{
//	/*
//	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
//	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
//	 * 都统一用这个优先级分组，千万不要再分组，切忌。
//	 */
//OLED_Init();				//OLED初始化
//	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

//	SysTick_Init(72);

//	/* 串口初始化	*/
//	USART_Config();
//  LED_Init();
//	DHT11_Init ();
//  OLED_Init(); 
//	
////	Capture_Config();
//  GELI_Init();
//	GELI_CodeInit();
//}

