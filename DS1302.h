#ifndef __DS1302_H__
#define __DS1302_H__

#include <iostm8s103k3.h>

//�ⲿ�ɵ���ʱ�����飬����0~6�ֱ�Ϊ�ꡢ�¡��ա�ʱ���֡��롢���ڣ�����Ϊ�з��ŵı���<0���ж�
extern char DS1302_Time[7];
extern char DS1302_Temp[7];

void DS1302_Init(void);
void DS1302_WriteByte(unsigned char Command,unsigned char Data);
char DS1302_ReadByte(unsigned char Command);
void DS1302_SetTime(void);
void DS1302_configTime(void);
void DS1302_ReadTime(void);

#endif
