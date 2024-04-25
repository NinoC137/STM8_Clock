#include <iostm8s103k3.h>
#include "DS1302.h"
#include "LED.h"
#include "KEY.h"
#include "BEEP.h"
#include "oled.h"
#include "DHT11.h"

unsigned char KeyNum=0,MODE=0,TimeSetSelect=0,TimeSetFlashFlag=0;

uint8_t temperature_u = 0, humidity_u = 0;          //�¶�, ʪ�����ݵ���������
uint8_t temperature_f = 0.0f, humidity_f = 0.0f;    //�¶�, ʪ�����ݵ�С������
uint8_t clock_hour = 12, clock_min = 30;

unsigned char Hzk[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char Hzk0[]=   //�����ֿ� "��"
{
0x00,0x20,0x18,0xC7,0x44,0x44,0x44,0x44,0xFC,0x44,0x44,0x44,0x44,0x04,0x00,0x00,
0x04,0x04,0x04,0x07,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
};

unsigned char Hzk1[]=   //�����ֿ� "��"
{
0x00,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,0x00,
0x80,0x40,0x30,0x0F,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x82,0x7F,0x00,0x00,0x00,
};

unsigned char Hzk2[]=   //�����ֿ� "��"
{
0x00,0x00,0x00,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x00,0x00,0x00,0x00,
};

unsigned char Hzk3[]=   //�����ֿ� "��"
{
0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,
};

unsigned char Hzk4[]=   //�����ֿ� "��"
{
0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,

0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,
};

unsigned char Hzk5[]=   //�����ֿ� "ʪ"
{
0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,

0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,
};

void  Fosc_Init(void)
{
    CLK_CKDIVR=0x01;    //1��Ƶ
}

void TimeShow(void)//ʱ����ʾ����
{
  OLED_ShowNum(18,0,DS1302_Time[0],2,16);//��
  OLED_ShowNum(55,0,DS1302_Time[1],2,16);//��
  OLED_ShowNum(95,0,DS1302_Time[2],2,16);//��
  OLED_ShowNum(0,2,DS1302_Time[3],2,16);//ʱ
  OLED_ShowNum(25,2,DS1302_Time[4],2,16);//��
  OLED_ShowNum(50,2,DS1302_Time[5],2,16);//��
  OLED_ShowNum(120,2,DS1302_Time[6],1,16);//����
}

void DHTShow(void)//��ʪ�����ݵ���ʾ
{
  OLED_ShowNum(0, 6, temperature_u, 2, 16);
  OLED_ShowNum(22, 6, temperature_f, 1, 16);
  OLED_ShowNum(80, 6, humidity_u, 2, 16);
  OLED_ShowNum(102, 6, humidity_f, 1, 16);
}

void TimeSet(void)//ʱ�����ù���
{
        if(KeyNum == 2)
        {
            TimeSetSelect++;
            TimeSetSelect%=7;   //DS1302_Time�����index,��0~6���л�
        }
        if(KeyNum == 3)
        {
          DS1302_Time[TimeSetSelect]++;
          if(DS1302_Time[0]>99){DS1302_Time[0]=0;}
          if(DS1302_Time[1]>12){DS1302_Time[1]=1;}
          if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
                    DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)
          {
            if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
          }
          else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || 
                    DS1302_Time[1]==11)
          {
            if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
          }
          else if(DS1302_Time[1]==2)
          {
                  if(DS1302_Time[0]%4==0)
                  {
                    if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
                  }
                  else
                  {
                    if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
                  }
          }
          if(DS1302_Time[3]>23){DS1302_Time[3]=0;}
          if(DS1302_Time[4]>59){DS1302_Time[4]=0;}
          if(DS1302_Time[5]>59){DS1302_Time[5]=0;}
          if(DS1302_Time[6]>7){DS1302_Time[6]=1;}
        }
            if(KeyNum==4)
	{
		DS1302_Time[TimeSetSelect]--;
                
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}
                
		if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
			  DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)
		{
                  if(DS1302_Time[2]<1){DS1302_Time[2]=31;}
                  if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || 
			  DS1302_Time[1]==11)
		{
                  if(DS1302_Time[2]<1){DS1302_Time[2]=30;}
                  if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)
			{
                          if(DS1302_Time[2]<1){DS1302_Time[2]=29;}
                          if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
			}
			else
			{
                          if(DS1302_Time[2]<1){DS1302_Time[2]=28;}
                          if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
			}
		}
                
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}
		if(DS1302_Time[6]<1){DS1302_Time[6]=7;}
	}
        
        if(TimeSetSelect==0 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(18,0,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(18,0,DS1302_Time[0],2,16);}
        
	if(TimeSetSelect==1 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(55,0,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(55,0,DS1302_Time[1],2,16);}
        
	if(TimeSetSelect==2 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(95,0,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(95,0,DS1302_Time[2],2,16);}
        
	if(TimeSetSelect==3 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(0,2,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(0,2,DS1302_Time[3],2,16);}
        
	if(TimeSetSelect==4 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(25,2,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(25,2,DS1302_Time[4],2,16);}
        
	if(TimeSetSelect==5 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(50,2,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(50,2,DS1302_Time[5],2,16);}
        
	if(TimeSetSelect==6 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(120,2,Hzk,sizeof(Hzk));}
	else {OLED_ShowNum(120,2,DS1302_Time[6],1,16);}
}

void  timer_Init(void)
{
    TIM1_PSCRH = 0;
    TIM1_PSCRL = 15;          //(15+1)��ƵΪ1M
    TIM1_ARRH = 0x0;
    TIM1_ARRL = 0x25;        //ÿ1ms�ж�һ��
    TIM1_IER = 0x01;         //��������ж�
    TIM1_CR1 = 0x01;         //������ʹ�ܣ���ʼ����
}

void main(void)
{    
    asm("sim");    //�����ж�
    
    Fosc_Init();        //ʱ�ӷ�Ƶ����
    timer_Init();       //��ʱ�ж�����
    LED_init();         //LED����
    Key_Init();         //������ʼ������
    BEEP_init();        //��������ʼ������
    DS1302_Init();      //DS1302��������
    DHT11_Init();       //��ʪ�ȴ���������
    OLED_Init();        //OLED����
    
    OLED_Clear();       //����
    DS1302_SetTime();   //����ʱ��
    
    OLED_ShowNum(1,0,20,2,16);
    OLED12864_ShowChianese(35,0,Hzk0,sizeof(Hzk0));     //��
    OLED12864_ShowChianese(75,0,Hzk1,sizeof(Hzk1));     //��
    OLED12864_ShowChianese(115,0,Hzk2,sizeof(Hzk2));    //��
    OLED12864_ShowChianese(17,2,Hzk3,sizeof(Hzk3));     //��
    OLED12864_ShowChianese(43,2,Hzk3,sizeof(Hzk3));     //��
    OLED_ShowString(80,2,"WEEK:");
    OLED_ShowString(16,6,".");          //�¶ȵ�С����
    OLED_ShowString(96,6,".");          //ʪ�ȵ�С����
    OLED12864_ShowChianese(0,4,Hzk4,sizeof(Hzk4));      //��
    OLED12864_ShowChianese(80,4,Hzk5,sizeof(Hzk5));     //ʪ
    OLED_ShowChar(114,6,'%',16);
    
    asm("rim");    //�����ж�

    while(1)
    {   
      KeyNum = KeyScan();
      
      if(KeyNum == 1)
      {
        switch(MODE)
        {
        case 0 :        //������ʾģʽ
          MODE = 1;
          TimeSetSelect=0;
          break;
        case 1:         //ʱ������ģʽ
          MODE = 2;
          break;
        case 2:         //��������ģʽ
          MODE = 0;     
          break;
        default:
          MODE = 0;
          break;
        }			
      }
      
      if(KeyNum == 5)   {  DS1302_SetTime(); }  //ʹ������Ч
      if(KeyNum == 6)   { BEEP_stop(); CLOCK_MODE = !CLOCK_MODE;}  //��������������,��رյ�ǰ����.
      
    }
}

void clock_task(uint8_t *beepFlag, uint8_t *targetDelay){
  static uint8_t timeCounter; //ÿ1ms��һ
  timeCounter++;
  
  if( DS1302_Time[3] == clock_hour && DS1302_Time[4] == clock_min){
    
    switch(*targetDelay){
    
    case 2:
      if(timeCounter <= *targetDelay){
        BEEP_stop();
        *beepFlag = 0;
      }else{
        timeCounter = 0;
        *targetDelay = 5;
      }
      break;
    
    case 5:
      if(timeCounter <= *targetDelay){
        BEEP_start();
        *beepFlag = 1;
      }else{
        timeCounter = 0;
        *targetDelay = 2;
      }
      break;
    
    default:
      *targetDelay = 2;
      break;
    }

  }
}

#pragma   vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_OVR_UIF(void)
{
    TIM1_SR1_UIF = 0;
    static uint16_t T0Count=0;
    static uint8_t beepDelay = 5;
    
    T0Count++;
    
    if(CLOCK_MODE == 1) {clock_task(&BEEP_STATUS, &beepDelay);}
      
    if(T0Count >= 5)
    {
      T0Count=0;
      DS1302_ReadTime();
      DHT11_Read_Data(&temperature_u,&humidity_u,&temperature_f,&humidity_f);
      
      TimeSetFlashFlag = !TimeSetFlashFlag;
      LED_Toggle();
      
      DHTShow();
      
      switch(MODE)
      {
      case 0:TimeShow();break;  //������ʾģʽ
      case 1:TimeSet();break;   //ʱ������ģʽ
      case 2: break;            //��������ģʽ
      }
      
    }
}
