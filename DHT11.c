#include <iostm8s103k3.h>
#include "stdbool.h"
#include "stdint.h"

//IO口操作
#define  DHT11_DQ_OUT PC_ODR_ODR1
#define  DHT11_DQ_IN  PC_IDR_IDR1

void delay_us(unsigned char us) 
{
  while(us--);
}

void  delay_ms1(unsigned int ms)
{
    unsigned int i,j;             //定义无符号整型变量i，j
  for(i=0;i<ms;i++)             //循环i自加，直到i=ms
    for(j=0;j<1600;j++);        //循环j自加，3164次
}

//IO方向设置
void DHT11_IO_IN(void)
{
    PC_DDR_DDR1=0;      //PC1为输入模式
    PC_CR1_C11=1;       //上拉电阻输入模式
    PC_CR2_C21=0;       //禁止外部中断
    DHT11_DQ_OUT=1;
}

void DHT11_IO_OUT(void)
{
    PC_DDR_DDR1=1;      //PC1为输出模式
    PC_CR1_C11=1;       //推挽输出
    PC_CR2_C21=0;       //2M输出
}

void DHT11_Rst(void)
{
    DHT11_IO_OUT(); //设置为输出
    DHT11_DQ_OUT=0; //拉低DQ
    delay_ms1(20);   //拉低至少18ms
    DHT11_DQ_OUT=1; //置高DQ
    delay_us(30);   //主机拉高20--40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
unsigned char DHT11_Check(void)
{
    //DHT输出80us低电平，作为应答信号
    //DHT输出80us高电平，通知处理器准备接收数据
    unsigned char retry=0;
    DHT11_IO_IN();//SET INPUT	 
    while (!DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
unsigned char DHT11_Read_Bit(void)
{
//数据为“0”格式：50us的低电平 + 26-28us的高电平
//数据为“1”格式：50us的低电平 + 70us的高电平
   unsigned char retry=0;
   DHT11_IO_IN();
   while(DHT11_DQ_IN && retry<100)//若为高电平说明上一位数据传输还未结束，等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
    while(!DHT11_DQ_IN && retry<100)//若为低电平，说明还未开始传输数据，等待变高电平
	{
		retry++;
		delay_us(1);
	}
    delay_us(40);//等待40us     0为 50us低电平 26--28us高电平    1为 50us低电平 70us高电平
    if(DHT11_DQ_IN) return 1;
    else  return 0;
}

//从DHT11读取一个字节
//返回值：读到的数据
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

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi,unsigned char *temp1,unsigned char *humi1)
{
    unsigned char buf[5]={0};
    unsigned char i;
    
    DHT11_Rst();
    
    if(DHT11_Check()==0)    //读取40位数据
    {
      for(i=0;i<5;i++)
      {
          buf[i]=DHT11_Read_Byte();
      }
      if(buf[0]+buf[1]+buf[2]+buf[3]==buf[4])//湿度整数 湿度小数 温度整数 温度小数 校验和
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

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在   
unsigned char DHT11_Init(void)
{
    DHT11_IO_OUT();
    DHT11_DQ_OUT=1;
    DHT11_Rst();
    return DHT11_Check();
}