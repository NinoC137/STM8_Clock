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
uint8_t clock_hour = 17, clock_min = 55;        //���ӵĶ�ʱ

unsigned char Hzk[]=    //�հ�ռλ, ���ڳ�����˸Ч��
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

unsigned char gImage_clock[128] = {     //����logoͼ�ο�
0X00,0X00,0X00,0XE0,0X30,0X18,0X08,0X88,0XC8,0X60,0X20,0X30,0X10,0X10,0X10,0X18,
0X18,0X10,0X10,0X10,0X30,0X20,0X60,0X48,0X88,0X08,0X18,0X30,0XE0,0X00,0X00,0X00,
0X00,0X00,0X00,0X80,0XF8,0X0E,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,
0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X0E,0XF8,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X01,0X1F,0X70,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0X03,0X06,0X0C,0X00,0X00,0X00,0X00,0X00,0XC0,0X70,0X1F,0X01,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X10,0X18,0X0D,0X06,0X06,0X04,0X0C,0X08,0X08,0X08,0X18,
0X18,0X08,0X08,0X08,0X0C,0X04,0X06,0X07,0X0D,0X18,0X00,0X00,0X00,0X00,0X00,0X00,
};

void  Fosc_Init(void)
{
  CLK_ICKR = 0x01; //Internal clock control register����λֵ��ʹ��HSIʱ�ӣ����ٻ��ѽ�ֹ
  CLK_ECKR = 0x00; //External clock control register��HSE��ֹ��
  CLK_SWR = 0xE1; //Clock master switch register��ѡ��HSIΪ��ʱ�ӡ�
  CLK_SWCR = 0x00; //Clock switch control register��ʱ���л���
  CLK_CKDIVR=0x01;    //1��Ƶ
  CLK_PCKENR1 = 0xFF; //Peripheral clock gating register 1��Fmaster������ʱ������
  CLK_PCKENR2 = 0x0; //ͬ��
  CLK_CSSR = 0x0; //Clock security system register��ʱ�Ӱ�ȫ���ر�
  CLK_CCOR = 0x0; //Configurable clock control register��ʱ������ر�
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
  if(KeyNum == 2)       //ѡ����һ��ʱ�����
  {
      TimeSetSelect++;
      TimeSetSelect%=7;   //DS1302_Time�����index,��0~6���л�
  }
  if(KeyNum == 3)       //������ѡ�еı�����1
  {
    DS1302_Temp[TimeSetSelect]++;       
    
    //������Ҫ���ʱ���ȡֵ��Χ��������
    if(DS1302_Temp[0]>99){DS1302_Temp[0]=0;}
    if(DS1302_Temp[1]>12){DS1302_Temp[1]=1;}
    if( DS1302_Temp[1]==1 || DS1302_Temp[1]==3 || DS1302_Temp[1]==5 || DS1302_Temp[1]==7 || 
              DS1302_Temp[1]==8 || DS1302_Temp[1]==10 || DS1302_Temp[1]==12)
    {
      if(DS1302_Temp[2]>31){DS1302_Temp[2]=1;}
    }
    else if(DS1302_Temp[1]==4 || DS1302_Temp[1]==6 || DS1302_Temp[1]==9 || 
              DS1302_Temp[1]==11)
    {
      if(DS1302_Temp[2]>30){DS1302_Temp[2]=1;}
    }
    else if(DS1302_Temp[1]==2)
    {
      if(DS1302_Temp[0]%4==0)
      {
        if(DS1302_Temp[2]>29){DS1302_Temp[2]=1;}
      }
      else
      {
        if(DS1302_Temp[2]>28){DS1302_Temp[2]=1;}
      }
    }
    if(DS1302_Temp[3]>23){DS1302_Temp[3]=0;}
    if(DS1302_Temp[4]>59){DS1302_Temp[4]=0;}
    if(DS1302_Temp[5]>59){DS1302_Temp[5]=0;}
    if(DS1302_Temp[6]>7){DS1302_Temp[6]=1;}
  }
  if(KeyNum==4)         //������ѡ�еı�����1
  {
    DS1302_Temp[TimeSetSelect]--; 
    
    if((signed char)DS1302_Temp[0]<0){DS1302_Temp[0]=99;}
    if(DS1302_Temp[1]<1){DS1302_Temp[1]=12;}
    
    if( DS1302_Temp[1]==1 || DS1302_Temp[1]==3 || DS1302_Temp[1]==5 || DS1302_Temp[1]==7 || 
              DS1302_Temp[1]==8 || DS1302_Temp[1]==10 || DS1302_Temp[1]==12)
    {
      if(DS1302_Temp[2]<1){DS1302_Temp[2]=31;}
      if(DS1302_Temp[2]>31){DS1302_Temp[2]=1;}
    }
    else if(DS1302_Temp[1]==4 || DS1302_Temp[1]==6 || DS1302_Temp[1]==9 || 
              DS1302_Temp[1]==11)
    {
      if(DS1302_Temp[2]<1){DS1302_Temp[2]=30;}
      if(DS1302_Temp[2]>30){DS1302_Temp[2]=1;}
    }
    else if(DS1302_Temp[1]==2)
    {
      if(DS1302_Temp[0]%4==0)
      {
        if(DS1302_Temp[2]<1){DS1302_Temp[2]=29;}
        if(DS1302_Temp[2]>29){DS1302_Temp[2]=1;}
      }
      else
      {
        if(DS1302_Temp[2]<1){DS1302_Temp[2]=28;}
        if(DS1302_Temp[2]>28){DS1302_Temp[2]=1;}
      }
    }
    
    if((signed char)DS1302_Temp[3]<0){DS1302_Temp[3]=23;}
    if((signed char)DS1302_Temp[4]<0){DS1302_Temp[4]=59;}
    if((signed char)DS1302_Temp[5]<0){DS1302_Temp[5]=59;}
    if(DS1302_Temp[6]<1){DS1302_Temp[6]=7;}
  }
  
  //�������ѡ����һ��ʱ��,��ô���������˸��ʶ
  if(TimeSetSelect==0 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(18,0,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(18,0,DS1302_Temp[0],2,16);}
  
  if(TimeSetSelect==1 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(55,0,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(55,0,DS1302_Temp[1],2,16);}
  
  if(TimeSetSelect==2 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(95,0,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(95,0,DS1302_Temp[2],2,16);}
  
  if(TimeSetSelect==3 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(0,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(0,2,DS1302_Temp[3],2,16);}
  
  if(TimeSetSelect==4 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(25,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(25,2,DS1302_Temp[4],2,16);}
  
  if(TimeSetSelect==5 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(50,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(50,2,DS1302_Temp[5],2,16);}
  
  if(TimeSetSelect==6 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(120,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(120,2,DS1302_Temp[6],1,16);}
}

void ClockSet(void){    //�������ù���
  static uint8_t clockSelcet = 0;
  
  if(KeyNum == 2)       //ѡ����һ������ʱ��
  {
      clockSelcet++;
      clockSelcet%=2;   //clockSelcet�����index,��3,4���л�
  }
  
  if(KeyNum == 3)       //ѡ�е�ʱ���1
  {
    DS1302_Temp[clockSelcet + 3]++;
    if(DS1302_Temp[3]>23){DS1302_Temp[3]=0;}
    if(DS1302_Temp[4]>59){DS1302_Temp[4]=0;}
  }
  
  if(KeyNum == 4)       //ѡ�е�ʱ���1
  {
    DS1302_Temp[clockSelcet + 3]--;
    if((signed char)DS1302_Temp[3]<0){DS1302_Temp[3]=23;}
    if((signed char)DS1302_Temp[4]<0){DS1302_Temp[4]=59;}
  }
  
  //�������ѡ����һ��ʱ��,��ô���������˸��ʶ
  if(clockSelcet==0 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(0,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(0,2,DS1302_Temp[3],2,16);}
  
  if(clockSelcet==1 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(25,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(25,2,DS1302_Temp[4],2,16);}
}

void  timer_Init(void)
{
  TIM1_PSCRH = 0;
  TIM1_PSCRL = 7;          //(7+1)��ƵΪ1M
  TIM1_ARRH = 0x0;
  TIM1_ARRL = 0x16;        //ÿ10ms�ж�һ��
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
  //DS1302_SetTime();   //����ʱ��,����һ�μ���,оƬ���Զ���ʱ
  
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
  OLED_ShowClock32x32Logo(42, 4, gImage_clock);       //����ģʽ

  asm("rim");    //�����ж�

  while(1)
  {
  }
}

void clock_task(uint8_t *beepFlag, uint16_t *targetDelay){
  static uint16_t timeCounter; //ÿ10ms��һ
  timeCounter++;
  
  if( DS1302_Time[3] == clock_hour && DS1302_Time[4] == clock_min){
    switch(*targetDelay){
    
    case 200:   //2��ػ�
      if(timeCounter <= *targetDelay){
        BEEP_stop();
        *beepFlag = 0;
      }else{
        timeCounter = 0;
        *targetDelay = 500;
      }
      break;
    
    case 500:   //5�����
      if(timeCounter <= *targetDelay){
        BEEP_start();
        *beepFlag = 1;
      }else{
        timeCounter = 0;
        *targetDelay = 200;
      }
      break;
    
    default:
      *targetDelay = 200;
      break;
    }

  }
}

void key_task(void){
  KeyNum = KeyScan();
  
  if(KeyNum == 1)
  {
    switch(MODE)
    {
    case 0 :        //������ʾģʽ
      for(int i = 0; i< 7; i++){
        DS1302_Temp[i] = DS1302_Time[i];
      }
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
 
  if(KeyNum == 5 && MODE == 1){ //ʹʱ��������Ч
    DS1302_configTime(); 
  }  
  
  if(KeyNum == 5 && MODE == 2){ //ʹ����������Ч
    clock_hour = DS1302_Temp[3];
    clock_min = DS1302_Temp[4];
    MODE = 0;
  }
  
  if(KeyNum == 6){  //�л����ӿ���,��������������,��رյ�ǰ����.
    BEEP_stop(); 
    CLOCK_MODE = !CLOCK_MODE;
    
    if(CLOCK_MODE == 1){        //������������,����ʾ����logo,��֮����ʾ
      OLED_ShowClock32x32Logo(42, 4, gImage_clock);
    }else{
      uint8_t clear[128] = {0x00};
      OLED_ShowClock32x32Logo(42, 4, clear);
    }
  }  
}

#pragma   vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_OVR_UIF(void)     //10ms����һ�ζ�ʱ���ж�
{
    TIM1_SR1_UIF = 0;   //����жϱ�ʶ
    static uint8_t timCount = 0;
    static uint16_t reFreshCount = 0;
    static uint16_t beepDelay = 500;

    timCount++;
    reFreshCount++;

    key_task();       //��ʱ����������
    
    if(CLOCK_MODE == 1) {
      clock_task(&BEEP_STATUS, &beepDelay);
    }

    if(timCount > 5){   //��ʱ���´���������
      timCount = 0;
      DS1302_ReadTime();
      DHT11_Read_Data(&temperature_u,&humidity_u,&temperature_f,&humidity_f);
    }
    
    if(reFreshCount >= 50){     //��ʱˢ��OLED��Ļ
      reFreshCount = 0;
      
      LED_Toggle();     // LED��˸
      
      TimeSetFlashFlag = !TimeSetFlashFlag;
      
      DHTShow();
      
      switch(MODE)
      {
      case 0:TimeShow(); break;  //������ʾģʽ
      case 1:TimeSet();break;   //ʱ������ģʽ
      case 2:ClockSet();break; //��������ģʽ
      default: TimeShow(); break;           
      }
    }    
}
