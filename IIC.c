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
    CLK_PCKENR1 |= 0x01; //使能IIC外设时钟
    PB_DDR_DDR4=1;//SCL
    PB_CR1_C14=1;
    PB_CR2_C24=1;
        
    PB_DDR_DDR5=1;//SDA
    PB_CR1_C15=1;
    PB_CR2_C25=1;
    
    I2C_CR1 = 0x00; //允许时钟延展，禁止广播呼叫，禁止iic
    I2C_FREQR = 0x01; //输入时钟频率8MHz（应该是1MHZ才对，可能是原创者之笔误）
    I2C_OARH = 0x40; //七位地址模式
    I2C_OARL = 0xa0; //自身地址0xa0
    I2C_CCRL = 0xff; //
    I2C_CCRH = 0x00; //标准模式
    I2C_TRISER = 0x02;
    I2C_CR1 |= 0x01; //使能iic外设

}
//这里使用的是STM8S105片上的IIC引脚PB4、PB5。另外就是需要使能IIC的时钟。
//发送和接收数据这里只给出一个最简单的例子，因为不同的外部芯片的通信方式不一样，不过一般都是：
//读操作：开始  -> 发送外设地址 -> 开始  ->发送需要读取的寄存器地址 -> 读一个字节  -> （可能再读一个字节）  ->。。。  -> 结束
//写操作：开始  -> 发送外设地址 -> 发送需要写入的寄存器地址 -> 写一个字节  -> （可能再写一个字节）  ->。。。  -> 结束

void IIC_Write_Byte(unsigned char DeviceAddress, unsigned char  Address, unsigned char  Data)

{
  unsigned char  temp = 0;

  while((I2C_SR3 & 0x02) != 0); //等待IIC总线空闲

  IIC_Start();

  while((I2C_SR1 & 0x01) == 0); //EV5，起始信号已经发送

  I2C_DR = (DeviceAddress & 0xfe); // 发送iic从器件物理地址，最低位0，写操作

  while((I2C_SR1 & 0x02) == 0); //地址已经被发送

  temp = I2C_SR1; //清除ADDR标志位

  temp = I2C_SR3;

  while((I2C_SR1 & 0x80) == 0); //等待发送寄存器为空

  I2C_DR = Address; //发送要写入的寄存器地址

  while((I2C_SR1 & 0x04) == 0); //等待发送完成

  while((I2C_SR1 & 0x80) == 0); //等待发送寄存器为空

  I2C_DR = Data; //发送要写入的数据

  while((I2C_SR1 & 0x04) == 0); //等待发送完成

  temp = I2C_SR1; //清零BTF标志位

  temp = I2C_DR;

  I2CStop(); //发送停止信号

}
 

unsigned char IIC_Read_Byte(unsigned char DeviceAddress, unsigned char Address)

{
    unsigned char temp = 0;

    short read_data = 0;

    while((I2C_SR3 & 0x02) != 0); //等待IIC总线空闲

    I2C_CR2 |= 0x04; //使能ACK

    I2CStart();

    while((I2C_SR1 & 0x01) == 0); //EV5，起始信号已经发送

    I2C_DR = (DeviceAddress & 0xfe); // 发送iic从器件物理地址，最低位0，写操作

    while((I2C_SR1 & 0x02) == 0); //地址已经被发送

    temp = I2C_SR1; //清除ADDR标志位

    temp = I2C_SR3;

    while((I2C_SR1 & 0x80) == 0); //等待发送寄存器为空

    I2C_DR = Address; // 发送要读取的寄存器地址

    while((I2C_SR1 & 0x04) == 0); //等待数据发送完成


    I2CStart();

    while((I2C_SR1 & 0x01) == 0); //EV5，起始信号已经发送

    I2C_DR = (DeviceAddress | 0x01); // 发送iic从器件物理地址，最低位1，读操作

    while((I2C_SR1 & 0x02) == 0); //地址已经被发送

    temp = I2C_SR1; //清除ADDR标志位

    temp = I2C_SR3;

    while((I2C_SR1 & 0x40) == 0); //等待接收数据寄存器非空

    read_data = I2C_DR;

    I2C_CR2 &= 0xfb; //读取数据下，发送stop必须禁止ack，才能释放从机

    temp = I2C_SR1; //清零BTF标志位

    temp = I2C_DR;

    I2CStop();

    return read_data;

}
void I2CStart(void)	//开始信号
{
	SCL = 1;
	SDA = 1;
	Delay_us(5);
	SDA = 0;   	
	Delay_us(5);
}
void I2CStop(void) //停止信号
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
  OLED_WR_Byte(0xAE,OLED_CMD);//–display off
  OLED_WR_Byte(0x00,OLED_CMD);//—set low column address
  OLED_WR_Byte(0x10,OLED_CMD);//—set high column address
  OLED_WR_Byte(0x40,OLED_CMD);//–set start line address
  OLED_WR_Byte(0xB0,OLED_CMD);//–set page address
  OLED_WR_Byte(0x81,OLED_CMD); // contract control
  OLED_WR_Byte(0xFF,OLED_CMD);//–128
  OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
  OLED_WR_Byte(0xA6,OLED_CMD);//–normal / reverse
  OLED_WR_Byte(0xA8,OLED_CMD);//–set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3F,OLED_CMD);//–1/32 duty
  OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
  OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
  OLED_WR_Byte(0x00,OLED_CMD);//
}

