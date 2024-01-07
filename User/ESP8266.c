#include "ESP8266.h"
#include "stdio.h"
#include "string.h"

 void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  
      while(i--) ;    
   }
}




//�����˼��Ѫ����������ַ���
char *message1="���㿴��������Ѫ����ϼ��ɣ�\r\n";



void ESP8266_Init1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
  USART_InitTypeDef USART_InitStructure; //���崮�ڽṹ��
	
  //ʹ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��ʱ��A
 
  //Ϊ��ʼ��������׼��
	
	
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//����PA2����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//�������ģʽΪ�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//��������ٶ�Ϊ50MHZ
  //��ʼ������PIN2
  GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��
  
	
	
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//����PA3����
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//�������ģʽΪ��������
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//��������ٶ�Ϊ50MHZ
  //��ʼ������PIN3��
  //GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��
  
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��ʱ��USART2
  
  USART_InitStructure.USART_BaudRate = 115200; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����֡����
  USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No; //��ʹ��У��λ 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Tx; //ģʽΪ����
  //��ʼ������2
  USART_Init(USART2, &USART_InitStructure); 
  

  //ʹ�ܴ���2
  USART_Cmd(USART2, ENABLE);
  
  
  }
  
  
  
  
  void ESP8266_Init2(void)
  {
  
   GPIO_InitTypeDef GPIO_InitStructure;//����ṹ��
   USART_InitTypeDef USART_InitStructure; //���崮�ڽṹ��
  
  
  //�����Ǵ���1
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��ʱ��A
 
  
  
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//����PA9����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//�������ģʽΪ�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//��������ٶ�Ϊ50MHZ

  GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��
  GPIO_SetBits(GPIOA,GPIO_Pin_9);
  
 // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//����PA10����
 // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//�������ģʽΪ��������
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//��������ٶ�Ϊ50MHZ
 
 // GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��
  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  
 
  USART_InitStructure.USART_BaudRate = 115200; //������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����֡����
  USART_InitStructure.USART_StopBits = USART_StopBits_1; //ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No; //��ʹ��У��λ 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Tx; //ģʽΪ����
  //��ʼ������1
  USART_Init(USART1, &USART_InitStructure); 
  //ʹ�ܴ���
  USART_Cmd(USART1, ENABLE);
  
  
  
}
//����1��2�Ļ��������ֽں���
void Usart1_SendByte(uint8_t Byte)
{

  USART_SendData(USART1,Byte);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );

}

//�ɻ����ֽڷ��ͺ�����װΪ�ַ������ͺ���
void Usart1_SendString(char *String)
{

 for(uint8_t i = 0;String[i] != '\0';i++)
  {
    Usart1_SendByte(String[i]);
  }


}




void Usart2_SendByte(uint8_t Byte)
{
  USART_SendData(USART2,Byte);
  while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET );
}


void Usart2_SendString(uint8_t *string)
{
  for(uint8_t i = 0;string[i] != '\0';i++)
  {
    Usart2_SendByte(string[i]);
  }
}

//������һ��ʾ���жϺ��� ��stm32 �ý��ջ���ͨ���ж���ʵ�ֶ��ڴ��ڽ��ܵ������ݵı��档
//ʹ���ж�֮ǰҪ�ȳ�ʼ�����ڽ����ж�
/*
#define buffer_size 512

uint8_t rx_buffer[buffer_size];
uint16_t rx_index=0;
uint8_t rx_data=0;

void USART_IRQHandler(void)
{
if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//�����Ƿ���յ����ݣ�
  {
    rx_data=USART_ReceiveData(USART1);//���ݶ�ȡ��rx_data��
	if(rx_index<buffer_size)//������ַ�뻺������ַ�Ĵ�С�Ա��жϻ�������û��
	{
	  rx_buffer[rx_index++]=rx_data;//���ոյõ��������ݵ�rx_data��ֵ�������������Һ�������index����С�໥��ϵ.
		
	}
	else//�������
    {
     rx_index=0;//�������
     memset(rx_buffer,0,buffer_size);	//rx_buffer�ڴ�ռ����,ȫ���滻Ϊ0x00
	}

  }
}

*/
/*
if(rx_index > 0)//��������0 ��֤������������������  ������Ҫע�����ݲ��ܳ���buffer_size ��󻺴��С��

{

  ����ط��ǽ��д���ĵط�

  rx_index=0;//ͬ�� ��������ͻ�����
  memset(rx_buffer,0,buffer_size);
}
*/
    void WIFI_Connect(void)//wifi���ӺͶ�Ӧ���ݵķ���
{
  
	
    Usart2_SendString("AT+RST\r\n");//rst ����01s
	delay_ms(1000);
	
    Usart2_SendString("AT+CWJAP=\"OPPO Find X\",\"894009338\"\r\n");//��������ֻ��ȵ�  ���ö���豸������ͬһ������
	delay_ms(4000);
	
  
	Usart2_SendString("AT+CIPSTART=\"TCP\",\"192.168.193.245\",8080\r\n");//���ӿɿ���tcpЭ�� ��01s��01s�����������豸���ӣ�Ҫ��ͬһ������
                             //�ɿ���tcp���Ӳ��ȶ�Ҳ����ʹ��udpЭ�� �����Ŭ��ʵ�����ݴ���
	delay_ms(4000);
	
	Usart2_SendString("AT+CIPMODE=1\r\n");//���ô���ģʽΪ͸��ģʽ
	delay_ms(1000);
	Usart2_SendString("AT+CIPSEND\r\n");//��������  ͸��ģʽ  ���ǲ��Ϸ��ʹ��ڻ�������  ����+++����
	delay_ms(2000);
	while(1)
	{	
    Usart2_SendString(message1);//����
	delay_ms(1000);
	}
}
