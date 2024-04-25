#include "iostm8s003f3.h"
#include "oled.h"
#include "oledfont.h"
//-------------------------------------------------------------- 
// Prototype : void I2C_Start(void) 
// Calls : Delay_5us() 
// Description : Start Singnal 
//-------------------------------------------------------------- 
void I2C_Init(void) 
{ 
  I2C_CR1 &= 0xFE; // PE=0, disable I2C module 
  SET_SCL_OUT(); 
  SET_SDA_OUT(); 
} 

//-------------------------------------------------------------- 
// Prototype : void Delay_5us(void) 
// Description : 大约延时5us 
//-------------------------------------------------------------- 
void Delay_5us(void) 
{ 
    uint8_t i; //fcpu 8MHz 时 
    for (i=18; i>0; i--); 
} 

void delayms(unsigned int ms)
{
  unsigned int i, j; 
  for(i=0;i<ms;i++)
    for(j=0;j<3164;j++);
}

//-------------------------------------------------------------- 
// Prototype : void I2C_Start(void) 
// Calls : Delay_5us() 
// Description : Start Singnal 
//-------------------------------------------------------------- 
void I2C_Start(void) 
{ 
// SDA 1->0 while SCL High 
    SDA = 1; 
    SCL = 1; 
    Delay_5us(); 
    SDA = 0; 
    Delay_5us(); 
    SCL = 0; 
} 

//-------------------------------------------------------------- 
// Prototype : void I2C_Stop(void) 
// Calls : Delay_5us() 
// Description : Stop Singnal 
//-------------------------------------------------------------- 
void I2C_Stop(void) 
{ 
  // SDA 0->1 while SCL High 
    SDA = 0; 
    SCL = 1; 
    Delay_5us(); 
    SDA = 1; 
    Delay_5us(); 
} 

//-------------------------------------------------------------- 
// Prototype : void I2C_SendACK(uint8_t ack); 
// Calls : Delay_5us() 
// Parameters : bit ack:1-noack, 0-ack 
// Description : Master device send ACK to slave device. 
//-------------------------------------------------------------- 
void I2C_SendACK(uint8_t ack) 
{ 
    if(ack == 0) 
    { 
    SDA = 0; 
    } 
    else 
    { 
    SDA = 1; 
    } 

    SCL = 1; 
    Delay_5us(); 

    SCL = 0; 
    Delay_5us(); 
} 

//-------------------------------------------------------------- 
// Prototype : uint8_t Write_IIC_Byte(uint8_t sendDAT) 
// Calls : Delay_5us() 
// Parameters : uint8_t sendDAT---data to be send 
// Return Value : CY--slave ack (1---noack，0---ack) 
// Description : Send one byte to I2C 
//-------------------------------------------------------------- 
uint8_t Write_IIC_Byte(uint8_t sendDAT) 
{ 
    uint8_t i,revack; 
    for (i= 0 ; i< 8; i++) 
    { 
        SCL = 0; 
        Delay_5us(); 
        if (sendDAT & 0x80) // write data 
        { 
          SDA = 1; 
        } 
        else 
        { 
          SDA = 0; 
        } 
        sendDAT <<= 1; 
        SCL = 1; 
        Delay_5us(); 
    } 

    SCL = 0; 
    Delay_5us(); 

    SET_SDA_IN(); 

    SCL = 1; 
    asm("nop"); 
    asm("nop"); 

    revack = (uint8_t)SDAM; 

    Delay_5us(); 
    SCL = 0; 

    SET_SDA_OUT(); 

    Delay_5us(); 
    return revack; 
} 


void Write_IIC_Command(unsigned char IIC_Command)
{
  I2C_Init();
  I2C_Start();
  Write_IIC_Byte(0x78); //Slave address,SA0=0
  Write_IIC_Byte(0x00); //write command
  Write_IIC_Byte(IIC_Command);
  I2C_Stop(); 
}


void Write_IIC_Data(unsigned char IIC_Data)
{
  I2C_Init();
  I2C_Start();
  Write_IIC_Byte(0x78); //Slave address,SA0=0
  Write_IIC_Byte(0x40); //write command 
  Write_IIC_Byte(IIC_Data);
  I2C_Stop(); 
}

void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
  if(cmd)
  {
    Write_IIC_Data(dat);
  }
  else 
  {
    Write_IIC_Command(dat);
  }
}

void fill_picture(unsigned char fill_Data)
{
  unsigned char m,n;
  for(m=0;m<8;m++)
  {
    OLED_WR_Byte(0xb0+m,0); //page0-page1
    OLED_WR_Byte(0x00,0); //low column start address
    OLED_WR_Byte(0x10,0); //high column start address
    for(n=0;n<128;n++)
    {
       OLED_WR_Byte(fill_Data,1);
    }
  }
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f),OLED_CMD);
}

void OLED_Init(void)
{
  
        I2C_Init();
	delayms(800);							//初始化之前的延时
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0x00,OLED_CMD);//设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);//设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);//设置起始行地址,集映射RAM显示起始行(0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//设置对比度控制寄存器
	OLED_WR_Byte(0xCF,OLED_CMD);//设置SEG输出电流亮度
        
	OLED_WR_Byte(0xA0,OLED_CMD);//段重定义设置,bit0:0,0->0;1,0->127; 0xa0左右反置 0xa1正常
        
	OLED_WR_Byte(0xC0,OLED_CMD);//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数   0xc0上下反置 0xc8正常
        
	OLED_WR_Byte(0xA6,OLED_CMD);//设置正常显示(设置显示方式;bit0:1,反相显示;0,正常显示	)
	OLED_WR_Byte(0xA8,OLED_CMD);//设置驱动路数 设置多路复用比(1比64)
	OLED_WR_Byte(0x3F,OLED_CMD);//1/64 duty(默认0X3F(1/64))
	OLED_WR_Byte(0xD3,OLED_CMD);//设置显示偏移位移映射RAM计数器(0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xD5,OLED_CMD);//设置显示时钟分频比/振荡器频率
	OLED_WR_Byte(0x80,OLED_CMD);//设置分频比，设置时钟为100帧/秒
	OLED_WR_Byte(0xD9,OLED_CMD);//设置预充电周期
	OLED_WR_Byte(0xF1,OLED_CMD);//设置预充15个时钟，放电1个时钟([3:0],PHASE 1;[7:4],PHASE 2;)
	OLED_WR_Byte(0xDA,OLED_CMD);//设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD);//[5:4]配置
	OLED_WR_Byte(0xDB,OLED_CMD);//设置VCOMH 电压倍率
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM 释放电压([6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;)
	OLED_WR_Byte(0x20,OLED_CMD);//设置页面寻址模式(0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0x8D,OLED_CMD);//设置充电泵启用/禁用
	OLED_WR_Byte(0x14,OLED_CMD);//设置(0x10禁用,0x14启用)
	OLED_WR_Byte(0xA4,OLED_CMD);// 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// 设置显示方式;bit0:1,反相显示;0,正常显示 (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//开启显示	
	OLED_Clear();
	OLED_Set_Pos(0,0);
}

void OLED_Clear(void)  
{  
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xB0+i,OLED_CMD);//设置页地址（0~7）
		OLED_WR_Byte(0x00,OLED_CMD);  //设置显示位置―列低地址
		OLED_WR_Byte(0x10,OLED_CMD);  //设置显示位置―列高地址 
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(0x00,OLED_DATA);
		}
	}
}

void OLED_On(void)
{
  unsigned char i,n;
  for(i=0;i<8;i++)
  {
    OLED_WR_Byte (0xb0+i,OLED_CMD); //设置页地址（0~7）
    OLED_WR_Byte (0x00,OLED_CMD); //设置显示位置—列低地址
    OLED_WR_Byte (0x10,OLED_CMD); //设置显示位置—列高地址
    for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
  } //更新显示
}

unsigned long int oled_pow(unsigned char m,unsigned char n)
{
    unsigned long int result=1;	 
    while(n--)result*=m;    
    return result;
}

void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{
  unsigned char c=0,i=0;
  c=chr-' ';
  if(x>Max_Column-1){x=0;y=y+2;}
  if(Char_Size ==16)
  {
    OLED_Set_Pos(x,y);
    for(i=0;i<8;i++)
    OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
    OLED_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
    OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
  }
  else 
  {
    OLED_Set_Pos(x,y);
    for(i=0;i<6;i++)
    OLED_WR_Byte(F6x8[c][i],OLED_DATA);
  }

}

void OLED_ShowChar2(unsigned char x,unsigned char y,unsigned char chr)
{      	
    unsigned char c=0,i=0;	
    c=chr-' ';//得到偏移后的值			
    if(x>Max_Column-1){x=0;y=y+2;}
    if(SIZE ==16)
    {
        OLED_Set_Pos(x,y);	
        for(i=0;i<8;i++)
        OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
        OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
    }
    else 
    {	
        OLED_Set_Pos(x,y+1);
        for(i=0;i<6;i++)
        OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
    }
}

void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long int num,unsigned char len,unsigned char size)
{         	
    unsigned char t,temp;
    unsigned char enshow=0;						   
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ',size);
                continue;
            }
            else enshow=1; 
        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
    }
}

//显示一个字符串
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char const *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {		
        OLED_ShowChar2(x,y,chr[j]);
        x+=8;
        if(x>120){x=0;y+=2;}
        j++;
    }
}

void OLED12864_ShowChianese(unsigned char x,unsigned char y,unsigned char *s,unsigned char len)
{
    static unsigned char i=0;
  
    len=len/2;//分上半部分和下半部分写入
    //先写上半部分8个像素
    OLED_Set_Pos(x,y);
    for(i=0;i<len;i++)
    {      
      Write_IIC_Data(*s++);
    }
    //再写下半部分4个像素
    OLED_Set_Pos(x,y+1);
    for(i=0;i<len;i++)
    {
      Write_IIC_Data(*s++);
    }
}