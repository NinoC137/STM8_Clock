#ifndef __DS1302_H__
#define __DS1302_H__

#include <iostm8s103k3.h>

//外部可调用时间数组，索引0~6分别为年、月、日、时、分、秒、星期，设置为有符号的便于<0的判断
extern char DS1302_Time[7];
extern char DS1302_Temp[7];

void DS1302_Init(void);
void DS1302_WriteByte(unsigned char Command,unsigned char Data);
char DS1302_ReadByte(unsigned char Command);
void DS1302_SetTime(void);
void DS1302_configTime(void);
void DS1302_ReadTime(void);

#endif
