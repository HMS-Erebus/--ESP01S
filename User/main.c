/*FreeRTOS���ͷ�ļ�*/
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


/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* esp8266������ */
static TaskHandle_t esp8266_Task_Handle = NULL;
/* max30102������ */
static TaskHandle_t max30102_Task_Handle = NULL;
/* ������������ */
static TaskHandle_t buzzer_Task_Handle = NULL;
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void esp8266_Task(void* pvParameters);/* esp8266_Task����ʵ�� */
static void max30102_Task(void* pvParameters);/* max30102_Task����ʵ�� */
static void buzzer_Task(void* pvParameters);/* buzzer_Task����ʵ�� */
//static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */
/*
*************����������***************
*/
int main(void)
{	
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    
  /* ������Ӳ����ʼ�� */
  //BSP_Init();
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);  /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����esp8266_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )esp8266_Task, /* ������ں��� */
                        (const char*    )"esp8266_Task",/* �������� */
                        (uint16_t       )64,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&esp8266_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
  
	/* ����max30102_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )max30102_Task, /* ������ں��� */
                        (const char*    )"max30102_Task",/* �������� */
                        (uint16_t       )64,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&max30102_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
	/* ����buzzer_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )buzzer_Task, /* ������ں��� */
                        (const char*    )"buzzer_Task",/* �������� */
                        (uint16_t       )64,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )4,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&buzzer_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)

	
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}
/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
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
	//�����豸�����ϱ�
	UsartPrintf(USART_DEBUG,"AT+MQTTSUB=0,\"/k0qbh9ZPUor/esp8266/user/get\",1\r\n");
	vTaskDelay(5000);
    while (1);
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void max30102_Task(void* parameter)
{	
	extern float Heart_Rate;
	extern float sp02_num;
	int i=0;
	//Delay_init(72);             //�����ⲿ����͵�Ƭ��ʱ��Ƶ��Ϊ72m
	OLED_Init();				  //OLED��ʼ��
	OLED_ShowString(1,1,"hello");
	Usart1_Init(115200);		  //���Դ��ڳ�ʼ��
	Usart2_Init(115200);		  //���ڳ�ʼ
	
	IIC_GPIO_INIT();		
	MAX30102_GPIO();
	Max30102_reset();
	MAX30102_Config();
	for(i = 0;i < 128;i++) 
	{
		while(MAX30102_INTPin_Read()==0)
		{
			//��ȡFIFO
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
//	OLED_Init();				//OLED��ʼ��
//	OLED_ShowString(1,1,"hello");
//	Usart1_Init(115200);		//���Դ��ڳ�ʼ��
//	Usart2_Init(115200);		//���ڳ�ʼ
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
//			//��ȡFIFO
//			max30102_read_fifo();
//		}
//	}
//	while (1)
//	{
//		//blood_Loop();				//��ȡѪ��������
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
//	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
//	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
//	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
//	 */
//OLED_Init();				//OLED��ʼ��
//	
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

//	SysTick_Init(72);

//	/* ���ڳ�ʼ��	*/
//	USART_Config();
//  LED_Init();
//	DHT11_Init ();
//  OLED_Init(); 
//	
////	Capture_Config();
//  GELI_Init();
//	GELI_CodeInit();
//}

