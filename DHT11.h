#ifndef __DHT11__H
#define __DHT11__H

unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi,unsigned char *temp1,unsigned char *humi1);
unsigned char DHT11_Init(void);

#endif 
