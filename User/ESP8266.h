#ifndef  _ESP8266_H//如果没有定义了则参加以下编译
#define  _ESP8266_H//一旦定义就有了定义 所以 其目的就是防止模块重复编译

#include "stm32f10x.h"
//#include "Delay.h"
#include "stdlib.h"
#include "string.h"

void ESP8266_Init1(void);
void ESP8266_Init2(void);
void Usart2_SendByte(uint8_t Byte);
void Usart2_SendString(uint8_t *string);
void Esp8266_SendString(uint8_t *string,uint16_t len);//串口2 发送字符串
void WIFI_Connect(void);
void Usart1_SendByte(uint8_t Byte);
void Usart1_SendString(char *String);
#endif  //结束编译
