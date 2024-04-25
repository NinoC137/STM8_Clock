#include <iostm8s103k3.h>
#include"OLED.h"

void  Delay_us(unsigned char us)
{      
    unsigned int i,j;
      for(i=0;i<us;i++);
        for(j=0;j<3;j++);
}


void IIC_Master_Init(void)

{
    CLK_PCKENR1 |= 0x01; //ʹ��IIC����ʱ��
    PB_DDR_DDR4=1;//SCL
    PB_CR1_C14=1;
    PB_CR2_C24=1;
        
    PB_DDR_DDR5=1;//SDA
    PB_CR1_C15=1;
    PB_CR2_C25=1;
    
    I2C_CR1 = 0x00; //����ʱ����չ����ֹ�㲥���У���ֹiic
    I2C_FREQR = 0x01; //����ʱ��Ƶ��8MHz��Ӧ����1MHZ�Ŷԣ�������ԭ����֮����
    I2C_OARH = 0x40; //��λ��ַģʽ
    I2C_OARL = 0xa0; //�����ַ0xa0
    I2C_CCRL = 0xff; //
    I2C_CCRH = 0x00; //��׼ģʽ
    I2C_TRISER = 0x02;
    I2C_CR1 |= 0x01; //ʹ��iic����

}
//����ʹ�õ���STM8S105Ƭ�ϵ�IIC����PB4��PB5�����������Ҫʹ��IIC��ʱ�ӡ�
//���ͺͽ�����������ֻ����һ����򵥵����ӣ���Ϊ��ͬ���ⲿоƬ��ͨ�ŷ�ʽ��һ��������һ�㶼�ǣ�
//����������ʼ  -> ���������ַ -> ��ʼ  ->������Ҫ��ȡ�ļĴ�����ַ -> ��һ���ֽ�  -> �������ٶ�һ���ֽڣ�  ->������  -> ����
//д��������ʼ  -> ���������ַ -> ������Ҫд��ļĴ�����ַ -> дһ���ֽ�  -> ��������дһ���ֽڣ�  ->������  -> ����

void IIC_Write_Byte(unsigned char DeviceAddress, unsigned char  Address, unsigned char  Data)

{
  unsigned char  temp = 0;

  while((I2C_SR3 & 0x02) != 0); //�ȴ�IIC���߿���

  IIC_Start();

  while((I2C_SR1 & 0x01) == 0); //EV5����ʼ�ź��Ѿ�����

  I2C_DR = (DeviceAddress & 0xfe); // ����iic�����������ַ�����λ0��д����

  while((I2C_SR1 & 0x02) == 0); //��ַ�Ѿ�������

  temp = I2C_SR1; //���ADDR��־λ

  temp = I2C_SR3;

  while((I2C_SR1 & 0x80) == 0); //�ȴ����ͼĴ���Ϊ��

  I2C_DR = Address; //����Ҫд��ļĴ�����ַ

  while((I2C_SR1 & 0x04) == 0); //�ȴ��������

  while((I2C_SR1 & 0x80) == 0); //�ȴ����ͼĴ���Ϊ��

  I2C_DR = Data; //����Ҫд�������

  while((I2C_SR1 & 0x04) == 0); //�ȴ��������

  temp = I2C_SR1; //����BTF��־λ

  temp = I2C_DR;

  I2CStop(); //����ֹͣ�ź�

}
 

unsigned char IIC_Read_Byte(unsigned char DeviceAddress, unsigned char Address)

{
    unsigned char temp = 0;

    short read_data = 0;

    while((I2C_SR3 & 0x02) != 0); //�ȴ�IIC���߿���

    I2C_CR2 |= 0x04; //ʹ��ACK

    I2CStart();

    while((I2C_SR1 & 0x01) == 0); //EV5����ʼ�ź��Ѿ�����

    I2C_DR = (DeviceAddress & 0xfe); // ����iic�����������ַ�����λ0��д����

    while((I2C_SR1 & 0x02) == 0); //��ַ�Ѿ�������

    temp = I2C_SR1; //���ADDR��־λ

    temp = I2C_SR3;

    while((I2C_SR1 & 0x80) == 0); //�ȴ����ͼĴ���Ϊ��

    I2C_DR = Address; // ����Ҫ��ȡ�ļĴ�����ַ

    while((I2C_SR1 & 0x04) == 0); //�ȴ����ݷ������


    I2CStart();

    while((I2C_SR1 & 0x01) == 0); //EV5����ʼ�ź��Ѿ�����

    I2C_DR = (DeviceAddress | 0x01); // ����iic�����������ַ�����λ1��������

    while((I2C_SR1 & 0x02) == 0); //��ַ�Ѿ�������

    temp = I2C_SR1; //���ADDR��־λ

    temp = I2C_SR3;

    while((I2C_SR1 & 0x40) == 0); //�ȴ��������ݼĴ����ǿ�

    read_data = I2C_DR;

    I2C_CR2 &= 0xfb; //��ȡ�����£�����stop�����ֹack�������ͷŴӻ�

    temp = I2C_SR1; //����BTF��־λ

    temp = I2C_DR;

    I2CStop();

    return read_data;

}
void I2CStart(void)	//��ʼ�ź�
{
	SCL = 1;
	SDA = 1;
	Delay_us(5);
	SDA = 0;   	
	Delay_us(5);
}
void I2CStop(void) //ֹͣ�ź�
{
    Delay_us(5);
	SCL = 1;
	SDA = 0;    
	Delay_us(5);
	SDA = 1;
}

OLED_initial()
{
  delay_ms(800);
  OLED_WR_Byte(0xAE,OLED_CMD);//�Cdisplay off
  OLED_WR_Byte(0x00,OLED_CMD);//��set low column address
  OLED_WR_Byte(0x10,OLED_CMD);//��set high column address
  OLED_WR_Byte(0x40,OLED_CMD);//�Cset start line address
  OLED_WR_Byte(0xB0,OLED_CMD);//�Cset page address
  OLED_WR_Byte(0x81,OLED_CMD); // contract control
  OLED_WR_Byte(0xFF,OLED_CMD);//�C128
  OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
  OLED_WR_Byte(0xA6,OLED_CMD);//�Cnormal / reverse
  OLED_WR_Byte(0xA8,OLED_CMD);//�Cset multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3F,OLED_CMD);//�C1/32 duty
  OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
  OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
  OLED_WR_Byte(0x00,OLED_CMD);//
}

