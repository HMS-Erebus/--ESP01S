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




//将病人检测血氧存入这个字符串
char *message1="让你看看我们热血的组合技吧！\r\n";



void ESP8266_Init1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
  USART_InitTypeDef USART_InitStructure; //定义串口结构体
	
  //使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能时钟A
 
  //为初始化函数做准备
	
	
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//设置PA2引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//设置输出模式为复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//设置输出速度为50MHZ
  //初始化函数PIN2
  GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
  
	
	
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//设置PA3引脚
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//设置输出模式为上拉输入
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//设置输出速度为50MHZ
  //初始化函数PIN3↓
  //GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
  
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能时钟USART2
  
  USART_InitStructure.USART_BaudRate = 115200; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数字帧长度
  USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位
  USART_InitStructure.USART_Parity = USART_Parity_No; //不使用校验位 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Tx; //模式为发送
  //初始化串口2
  USART_Init(USART2, &USART_InitStructure); 
  

  //使能串口2
  USART_Cmd(USART2, ENABLE);
  
  
  }
  
  
  
  
  void ESP8266_Init2(void)
  {
  
   GPIO_InitTypeDef GPIO_InitStructure;//定义结构体
   USART_InitTypeDef USART_InitStructure; //定义串口结构体
  
  
  //下面是串口1
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能时钟A
 
  
  
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//设置PA9引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;//设置输出模式为复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//设置输出速度为50MHZ

  GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
  GPIO_SetBits(GPIOA,GPIO_Pin_9);
  
 // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//设置PA10引脚
 // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//设置输出模式为上拉输入
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;//设置输出速度为50MHZ
 
 // GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  
 
  USART_InitStructure.USART_BaudRate = 115200; //波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数字帧长度
  USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位
  USART_InitStructure.USART_Parity = USART_Parity_No; //不使用校验位 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Tx; //模式为发送
  //初始化串口1
  USART_Init(USART1, &USART_InitStructure); 
  //使能串口
  USART_Cmd(USART1, ENABLE);
  
  
  
}
//串口1和2的基础发送字节函数
void Usart1_SendByte(uint8_t Byte)
{

  USART_SendData(USART1,Byte);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );

}

//由基础字节发送函数封装为字符串发送函数
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

//下面是一个示例中断函数 在stm32 得接收机上通过中断来实现对于串口接受到的数据的保存。
//使用中断之前要先初始化串口接受中断
/*
#define buffer_size 512

uint8_t rx_buffer[buffer_size];
uint16_t rx_index=0;
uint8_t rx_data=0;

void USART_IRQHandler(void)
{
if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//串口是否接收到数据？
  {
    rx_data=USART_ReceiveData(USART1);//数据读取到rx_data中
	if(rx_index<buffer_size)//索引地址与缓冲区地址的大小对比判断缓冲区满没满
	{
	  rx_buffer[rx_index++]=rx_data;//讲刚刚得到串口数据的rx_data赋值到缓冲区，并且和索引（index）大小相互联系.
		
	}
	else//如果满了
    {
     rx_index=0;//索引清空
     memset(rx_buffer,0,buffer_size);	//rx_buffer内存空间清空,全部替换为0x00
	}

  }
}

*/
/*
if(rx_index > 0)//索引大于0 则证明缓冲区里面有数据  ，但是要注意数据不能超过buffer_size 最大缓存大小。

{

  这个地方是进行处理的地方

  rx_index=0;//同上 清空索引和缓存区
  memset(rx_buffer,0,buffer_size);
}
*/
    void WIFI_Connect(void)//wifi连接和对应数据的发送
{
  
	
    Usart2_SendString("AT+RST\r\n");//rst 重启01s
	delay_ms(1000);
	
    Usart2_SendString("AT+CWJAP=\"OPPO Find X\",\"894009338\"\r\n");//连入你的手机热点  ，让多个设备保持在同一网段下
	delay_ms(4000);
	
  
	Usart2_SendString("AT+CIPSTART=\"TCP\",\"192.168.193.245\",8080\r\n");//连接可靠得tcp协议 让01s和01s或者与其他设备连接，要在同一网段下
                             //可靠的tcp连接不稳定也可以使用udp协议 尽最大努力实现数据传输
	delay_ms(4000);
	
	Usart2_SendString("AT+CIPMODE=1\r\n");//设置传输模式为透传模式
	delay_ms(1000);
	Usart2_SendString("AT+CIPSEND\r\n");//发送数据  透传模式  就是不断发送串口缓存数据  发送+++则解除
	delay_ms(2000);
	while(1)
	{	
    Usart2_SendString(message1);//数据
	delay_ms(1000);
	}
}
