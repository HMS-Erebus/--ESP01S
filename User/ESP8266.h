#ifndef  _ESP8266_H//���û�ж�������μ����±���
#define  _ESP8266_H//һ����������˶��� ���� ��Ŀ�ľ��Ƿ�ֹģ���ظ�����

#include "stm32f10x.h"
//#include "Delay.h"
#include "stdlib.h"
#include "string.h"

void ESP8266_Init1(void);
void ESP8266_Init2(void);
void Usart2_SendByte(uint8_t Byte);
void Usart2_SendString(uint8_t *string);
void Esp8266_SendString(uint8_t *string,uint16_t len);//����2 �����ַ���
void WIFI_Connect(void);
void Usart1_SendByte(uint8_t Byte);
void Usart1_SendString(char *String);
#endif  //��������
