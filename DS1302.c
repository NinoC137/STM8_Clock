#include "DS1302.H"

//引脚定义
#define DS1302_CE       PB_ODR_ODR0
#define DS1302_IO_OUT   PB_ODR_ODR1     //DS1302输入输出端口（输出）
#define DS1302_IO_IN    PB_IDR_IDR1     //DS1302输入输出端口（输入）
#define DS1302_SCLK     PB_ODR_ODR2

//寄存器写入地址/指令定义
#define DS1302_SECOND		0x80
#define DS1302_MINUTE		0x82
#define DS1302_HOUR		0x84
#define DS1302_DATE		0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY		0x8A
#define DS1302_YEAR		0x8C
#define DS1302_WP		0x8E
#define DS1302_CHARGER          0x90 

void delay(void)
{
    asm("nop");
}

/****************************************************************/
//DS1302芯片IO引脚方向性配置函数DS1302_IO_DDR()，有形参ddr无返回值
/****************************************************************/
void DS1302_IO_DDR(unsigned char ddr)
{
      if(ddr==1)//配置为输出方式
      {
        PB_DDR_DDR1=1;//配置PC1引脚为输出引脚
        PB_CR1_C11=1;//配置PC1引脚为推挽输出模式
        PB_CR2_C21=0;//配置PC1引脚低速率输出模式
      }
      else//配置为输入方式
      {
        PB_DDR_DDR1=0;//配置PC1引脚为输入引脚
        PB_CR1_C11=1;//配置PC1引脚为弱上拉输入模式
        PB_CR2_C21=0;//禁止PC1引脚外部中断功能
      }
}

//时间数组，索引0~6分别为年、月、日、时、分、秒、星期，设置为有符号的便于<0的判断
char DS1302_Time[]={24,4,25,17,55,30,4};

/**
  * @brief  DS1302初始化
  * @param  无
  * @retval 无
  */
void DS1302_Init(void)
{       
        PB_DDR_DDR0 = 1;        //设置输出模式
        PB_CR1_C10 = 1;         //设置推挽输出
        PB_CR2_C20 = 0;         //设置2MHZ
        
        PB_DDR_DDR2 = 1;        //设置输出模式
        PB_CR1_C12 = 1;         //设置推挽输出
        PB_CR2_C22 = 0;         //设置2MHZ
        
	DS1302_CE=0;
	DS1302_SCLK=0;
}

/**
  * @brief  DS1302写一个字节
  * @param  Command 命令字/地址
  * @param  Data 要写入的数据
  * @retval 无
  */
void DS1302_WriteByte(unsigned char Command,unsigned char Data)
{
      char i;
      DS1302_IO_DDR(1);//配置DS1302_IO引脚为输出方式
      DS1302_CE=1;//启动DS1302总线
      //写入目标地址：addr
      Command=Command&0xFE;//最低位置零，寄存器0位为0时写，为1时读
      for(i=0;i<8;i++)
      {
        
        if(Command&0x01)        DS1302_IO_OUT=1;
        else                    DS1302_IO_OUT=0;
          
        DS1302_SCLK=1;//产生时钟
        DS1302_SCLK=0;
        Command=Command>>1;
      }	
      //写入数据：data
      for(i=0;i<8;i++)
      {
        
        if(Data&0x01)   DS1302_IO_OUT=1;
        else            DS1302_IO_OUT=0;
          
        DS1302_SCLK=1;//产生时钟
        DS1302_SCLK=0;
        Data=Data>>1;
      }
      DS1302_CE=0;//停止DS1302总线
}

/**
  * @brief  DS1302读一个字节
  * @param  Command 命令字/地址
  * @retval 读出的数据
  */
char DS1302_ReadByte(unsigned char Command)
{
         char i,temp;//i为循环控制变量，temp为读取回单片机的数据	
          DS1302_IO_DDR(1);//配置DS1302_IO引脚为输出方式
          DS1302_CE=1;//启动DS1302总线
          //写入目标地址：addr
          Command=Command|0x01;//最低位置高，寄存器0位为0时写，为1时读
          for(i=0;i<8;i++)
          {
            if(Command&0x01)//送出地址
              DS1302_IO_OUT=1;
            else
              DS1302_IO_OUT=0;
            DS1302_SCLK=1;//产生时钟
            DS1302_SCLK=0;
            Command=Command>>1;
          }	
          //输出数据：temp
          DS1302_IO_DDR(0);//配置DS1302_IO引脚为输入方式
          for(i=0;i<8;i++) 
          {
            temp=temp>>1;
            if(DS1302_IO_IN)//取回IO线路状态
              temp|=0x80;//数据位为“1”时
            else
              temp&=0x7F;//数据位为“0”时
            DS1302_SCLK=1;//产生时钟
            DS1302_SCLK=0;
          }	
          DS1302_CE=0;//停止DS1302总线
          return temp;//返回读取数据temp
}
/**
  * @brief  DS1302设置时间，调用之后，DS1302_Time数组的数字会被设置到DS1302中
  * @param  无
  * @retval 无
  */
void DS1302_SetTime(void)
{
	DS1302_WriteByte(DS1302_WP,0x00);                                       //关闭写保护
        DS1302_WriteByte(DS1302_SECOND,0X80);                                   //暂停时钟
        DS1302_WriteByte(DS1302_CHARGER,0x90);                                  //涓流充电
        //十进制转BCD码后写入
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);   //年
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);  //月
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);   //日
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);   //时
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10); //分
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10); //秒
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);    //星期
	DS1302_WriteByte(DS1302_WP,0x80);       //打开写保护
}

/**
  * @brief  DS1302读取时间，调用之后，DS1302中的数据会被读取到DS1302_Time数组中
  * @param  无
  * @retval 无
  */
void DS1302_ReadTime(void)
{
	char Temp=0;
	Temp=DS1302_ReadByte(DS1302_YEAR);      //年
	DS1302_Time[0]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_MONTH);     //月
	DS1302_Time[1]=Temp/16*10+Temp%16;    
	Temp=DS1302_ReadByte(DS1302_DATE);      //日
	DS1302_Time[2]=Temp/16*10+Temp%16;    
	Temp=DS1302_ReadByte(DS1302_HOUR);      //时
	DS1302_Time[3]=Temp/16*10+Temp%16;     
	Temp=DS1302_ReadByte(DS1302_MINUTE);    //分
	DS1302_Time[4]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_SECOND);    //秒
	DS1302_Time[5]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_DAY);       //星期
	DS1302_Time[6]=Temp/16*10+Temp%16;
}
