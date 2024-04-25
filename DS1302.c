#include "DS1302.H"

//���Ŷ���
#define DS1302_CE       PB_ODR_ODR0
#define DS1302_IO_OUT   PB_ODR_ODR1     //DS1302��������˿ڣ������
#define DS1302_IO_IN    PB_IDR_IDR1     //DS1302��������˿ڣ����룩
#define DS1302_SCLK     PB_ODR_ODR2

//�Ĵ���д���ַ/ָ���
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
//DS1302оƬIO���ŷ��������ú���DS1302_IO_DDR()�����β�ddr�޷���ֵ
/****************************************************************/
void DS1302_IO_DDR(unsigned char ddr)
{
      if(ddr==1)//����Ϊ�����ʽ
      {
        PB_DDR_DDR1=1;//����PC1����Ϊ�������
        PB_CR1_C11=1;//����PC1����Ϊ�������ģʽ
        PB_CR2_C21=0;//����PC1���ŵ��������ģʽ
      }
      else//����Ϊ���뷽ʽ
      {
        PB_DDR_DDR1=0;//����PC1����Ϊ��������
        PB_CR1_C11=1;//����PC1����Ϊ����������ģʽ
        PB_CR2_C21=0;//��ֹPC1�����ⲿ�жϹ���
      }
}

//ʱ�����飬����0~6�ֱ�Ϊ�ꡢ�¡��ա�ʱ���֡��롢���ڣ�����Ϊ�з��ŵı���<0���ж�
char DS1302_Time[]={24,4,25,17,55,30,4};

/**
  * @brief  DS1302��ʼ��
  * @param  ��
  * @retval ��
  */
void DS1302_Init(void)
{       
        PB_DDR_DDR0 = 1;        //�������ģʽ
        PB_CR1_C10 = 1;         //�����������
        PB_CR2_C20 = 0;         //����2MHZ
        
        PB_DDR_DDR2 = 1;        //�������ģʽ
        PB_CR1_C12 = 1;         //�����������
        PB_CR2_C22 = 0;         //����2MHZ
        
	DS1302_CE=0;
	DS1302_SCLK=0;
}

/**
  * @brief  DS1302дһ���ֽ�
  * @param  Command ������/��ַ
  * @param  Data Ҫд�������
  * @retval ��
  */
void DS1302_WriteByte(unsigned char Command,unsigned char Data)
{
      char i;
      DS1302_IO_DDR(1);//����DS1302_IO����Ϊ�����ʽ
      DS1302_CE=1;//����DS1302����
      //д��Ŀ���ַ��addr
      Command=Command&0xFE;//���λ���㣬�Ĵ���0λΪ0ʱд��Ϊ1ʱ��
      for(i=0;i<8;i++)
      {
        
        if(Command&0x01)        DS1302_IO_OUT=1;
        else                    DS1302_IO_OUT=0;
          
        DS1302_SCLK=1;//����ʱ��
        DS1302_SCLK=0;
        Command=Command>>1;
      }	
      //д�����ݣ�data
      for(i=0;i<8;i++)
      {
        
        if(Data&0x01)   DS1302_IO_OUT=1;
        else            DS1302_IO_OUT=0;
          
        DS1302_SCLK=1;//����ʱ��
        DS1302_SCLK=0;
        Data=Data>>1;
      }
      DS1302_CE=0;//ֹͣDS1302����
}

/**
  * @brief  DS1302��һ���ֽ�
  * @param  Command ������/��ַ
  * @retval ����������
  */
char DS1302_ReadByte(unsigned char Command)
{
         char i,temp;//iΪѭ�����Ʊ�����tempΪ��ȡ�ص�Ƭ��������	
          DS1302_IO_DDR(1);//����DS1302_IO����Ϊ�����ʽ
          DS1302_CE=1;//����DS1302����
          //д��Ŀ���ַ��addr
          Command=Command|0x01;//���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
          for(i=0;i<8;i++)
          {
            if(Command&0x01)//�ͳ���ַ
              DS1302_IO_OUT=1;
            else
              DS1302_IO_OUT=0;
            DS1302_SCLK=1;//����ʱ��
            DS1302_SCLK=0;
            Command=Command>>1;
          }	
          //������ݣ�temp
          DS1302_IO_DDR(0);//����DS1302_IO����Ϊ���뷽ʽ
          for(i=0;i<8;i++) 
          {
            temp=temp>>1;
            if(DS1302_IO_IN)//ȡ��IO��·״̬
              temp|=0x80;//����λΪ��1��ʱ
            else
              temp&=0x7F;//����λΪ��0��ʱ
            DS1302_SCLK=1;//����ʱ��
            DS1302_SCLK=0;
          }	
          DS1302_CE=0;//ֹͣDS1302����
          return temp;//���ض�ȡ����temp
}
/**
  * @brief  DS1302����ʱ�䣬����֮��DS1302_Time��������ֻᱻ���õ�DS1302��
  * @param  ��
  * @retval ��
  */
void DS1302_SetTime(void)
{
	DS1302_WriteByte(DS1302_WP,0x00);                                       //�ر�д����
        DS1302_WriteByte(DS1302_SECOND,0X80);                                   //��ͣʱ��
        DS1302_WriteByte(DS1302_CHARGER,0x90);                                  //������
        //ʮ����תBCD���д��
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);   //��
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);  //��
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);   //��
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);   //ʱ
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10); //��
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10); //��
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);    //����
	DS1302_WriteByte(DS1302_WP,0x80);       //��д����
}

/**
  * @brief  DS1302��ȡʱ�䣬����֮��DS1302�е����ݻᱻ��ȡ��DS1302_Time������
  * @param  ��
  * @retval ��
  */
void DS1302_ReadTime(void)
{
	char Temp=0;
	Temp=DS1302_ReadByte(DS1302_YEAR);      //��
	DS1302_Time[0]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_MONTH);     //��
	DS1302_Time[1]=Temp/16*10+Temp%16;    
	Temp=DS1302_ReadByte(DS1302_DATE);      //��
	DS1302_Time[2]=Temp/16*10+Temp%16;    
	Temp=DS1302_ReadByte(DS1302_HOUR);      //ʱ
	DS1302_Time[3]=Temp/16*10+Temp%16;     
	Temp=DS1302_ReadByte(DS1302_MINUTE);    //��
	DS1302_Time[4]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_SECOND);    //��
	DS1302_Time[5]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByte(DS1302_DAY);       //����
	DS1302_Time[6]=Temp/16*10+Temp%16;
}
