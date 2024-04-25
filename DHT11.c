#include <iostm8s103k3.h>
#include "stdbool.h"
#include "stdint.h"

//IO�ڲ���
#define  DHT11_DQ_OUT PC_ODR_ODR1
#define  DHT11_DQ_IN  PC_IDR_IDR1

void delay_us(unsigned char us) 
{
  while(us--);
}

void  delay_ms1(unsigned int ms)
{
    unsigned int i,j;             //�����޷������ͱ���i��j
  for(i=0;i<ms;i++)             //ѭ��i�Լӣ�ֱ��i=ms
    for(j=0;j<1600;j++);        //ѭ��j�Լӣ�3164��
}

//IO��������
void DHT11_IO_IN(void)
{
    PC_DDR_DDR1=0;      //PC1Ϊ����ģʽ
    PC_CR1_C11=1;       //������������ģʽ
    PC_CR2_C21=0;       //��ֹ�ⲿ�ж�
    DHT11_DQ_OUT=1;
}

void DHT11_IO_OUT(void)
{
    PC_DDR_DDR1=1;      //PC1Ϊ���ģʽ
    PC_CR1_C11=1;       //�������
    PC_CR2_C21=0;       //2M���
}

void DHT11_Rst(void)
{
    DHT11_IO_OUT(); //����Ϊ���
    DHT11_DQ_OUT=0; //����DQ
    delay_ms1(20);   //��������18ms
    DHT11_DQ_OUT=1; //�ø�DQ
    delay_us(30);   //��������20--40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
unsigned char DHT11_Check(void)
{
    //DHT���80us�͵�ƽ����ΪӦ���ź�
    //DHT���80us�ߵ�ƽ��֪ͨ������׼����������
    unsigned char retry=0;
    DHT11_IO_IN();//SET INPUT	 
    while (!DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
unsigned char DHT11_Read_Bit(void)
{
//����Ϊ��0����ʽ��50us�ĵ͵�ƽ + 26-28us�ĸߵ�ƽ
//����Ϊ��1����ʽ��50us�ĵ͵�ƽ + 70us�ĸߵ�ƽ
   unsigned char retry=0;
   DHT11_IO_IN();
   while(DHT11_DQ_IN && retry<100)//��Ϊ�ߵ�ƽ˵����һλ���ݴ��仹δ�������ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
    while(!DHT11_DQ_IN && retry<100)//��Ϊ�͵�ƽ��˵����δ��ʼ�������ݣ��ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
    delay_us(40);//�ȴ�40us     0Ϊ 50us�͵�ƽ 26--28us�ߵ�ƽ    1Ϊ 50us�͵�ƽ 70us�ߵ�ƽ
    if(DHT11_DQ_IN) return 1;
    else  return 0;
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
unsigned char DHT11_Read_Byte(void)
{
    unsigned char i,dat=0;
    DHT11_IO_IN();
    for(i=0;i<8;i++)
    {
        dat<<=1;
        dat|=DHT11_Read_Bit();
    }
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi,unsigned char *temp1,unsigned char *humi1)
{
    unsigned char buf[5]={0};
    unsigned char i;
    
    DHT11_Rst();
    
    if(DHT11_Check()==0)    //��ȡ40λ����
    {
      for(i=0;i<5;i++)
      {
          buf[i]=DHT11_Read_Byte();
      }
      if(buf[0]+buf[1]+buf[2]+buf[3]==buf[4])//ʪ������ ʪ��С�� �¶����� �¶�С�� У���
      {
          *humi=buf[0];
          *humi1=buf[1];
          *temp=buf[2];
          *temp1=buf[3];
      }
    }
    else return 1;
  return 0;
}

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����   
unsigned char DHT11_Init(void)
{
    DHT11_IO_OUT();
    DHT11_DQ_OUT=1;
    DHT11_Rst();
    return DHT11_Check();
}