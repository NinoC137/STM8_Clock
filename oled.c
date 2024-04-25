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
// Description : ��Լ��ʱ5us 
//-------------------------------------------------------------- 
void Delay_5us(void) 
{ 
    uint8_t i; //fcpu 8MHz ʱ 
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
// Return Value : CY--slave ack (1---noack��0---ack) 
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
	delayms(800);							//��ʼ��֮ǰ����ʱ
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0x00,OLED_CMD);//���õ��е�ַ
	OLED_WR_Byte(0x10,OLED_CMD);//���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);//������ʼ�е�ַ,��ӳ��RAM��ʾ��ʼ��(0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//���öԱȶȿ��ƼĴ���
	OLED_WR_Byte(0xCF,OLED_CMD);//����SEG�����������
        
	OLED_WR_Byte(0xA0,OLED_CMD);//���ض�������,bit0:0,0->0;1,0->127; 0xa0���ҷ��� 0xa1����
        
	OLED_WR_Byte(0xC0,OLED_CMD);//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��   0xc0���·��� 0xc8����
        
	OLED_WR_Byte(0xA6,OLED_CMD);//����������ʾ(������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	)
	OLED_WR_Byte(0xA8,OLED_CMD);//��������·�� ���ö�·���ñ�(1��64)
	OLED_WR_Byte(0x3F,OLED_CMD);//1/64 duty(Ĭ��0X3F(1/64))
	OLED_WR_Byte(0xD3,OLED_CMD);//������ʾƫ��λ��ӳ��RAM������(0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xD5,OLED_CMD);//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WR_Byte(0x80,OLED_CMD);//���÷�Ƶ�ȣ�����ʱ��Ϊ100֡/��
	OLED_WR_Byte(0xD9,OLED_CMD);//����Ԥ�������
	OLED_WR_Byte(0xF1,OLED_CMD);//����Ԥ��15��ʱ�ӣ��ŵ�1��ʱ��([3:0],PHASE 1;[7:4],PHASE 2;)
	OLED_WR_Byte(0xDA,OLED_CMD);//����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD);//[5:4]����
	OLED_WR_Byte(0xDB,OLED_CMD);//����VCOMH ��ѹ����
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM �ͷŵ�ѹ([6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;)
	OLED_WR_Byte(0x20,OLED_CMD);//����ҳ��Ѱַģʽ(0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0x8D,OLED_CMD);//���ó�������/����
	OLED_WR_Byte(0x14,OLED_CMD);//����(0x10����,0x14����)
	OLED_WR_Byte(0xA4,OLED_CMD);// ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//������ʾ	
	OLED_Clear();
	OLED_Set_Pos(0,0);
}

void OLED_Clear(void)  
{  
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xB0+i,OLED_CMD);//����ҳ��ַ��0~7��
		OLED_WR_Byte(0x00,OLED_CMD);  //������ʾλ�èD�е͵�ַ
		OLED_WR_Byte(0x10,OLED_CMD);  //������ʾλ�èD�иߵ�ַ 
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
    OLED_WR_Byte (0xb0+i,OLED_CMD); //����ҳ��ַ��0~7��
    OLED_WR_Byte (0x00,OLED_CMD); //������ʾλ�á��е͵�ַ
    OLED_WR_Byte (0x10,OLED_CMD); //������ʾλ�á��иߵ�ַ
    for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
  } //������ʾ
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
    c=chr-' ';//�õ�ƫ�ƺ��ֵ			
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

//��ʾһ���ַ���
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
  
    len=len/2;//���ϰ벿�ֺ��°벿��д��
    //��д�ϰ벿��8������
    OLED_Set_Pos(x,y);
    for(i=0;i<len;i++)
    {      
      Write_IIC_Data(*s++);
    }
    //��д�°벿��4������
    OLED_Set_Pos(x,y+1);
    for(i=0;i<len;i++)
    {
      Write_IIC_Data(*s++);
    }
}