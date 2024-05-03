#include <iostm8s103k3.h>
#include "DS1302.h"
#include "LED.h"
#include "KEY.h"
#include "BEEP.h"
#include "oled.h"
#include "DHT11.h"

unsigned char KeyNum=0,MODE=0,TimeSetSelect=0,TimeSetFlashFlag=0;

uint8_t temperature_u = 0, humidity_u = 0;          //温度, 湿度数据的整数部分
uint8_t temperature_f = 0.0f, humidity_f = 0.0f;    //温度, 湿度数据的小数部分
uint8_t clock_hour = 17, clock_min = 55;        //闹钟的定时

unsigned char Hzk[]=    //空白占位, 用于呈现闪烁效果
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char Hzk0[]=   //汉字字库 "年"
{
0x00,0x20,0x18,0xC7,0x44,0x44,0x44,0x44,0xFC,0x44,0x44,0x44,0x44,0x04,0x00,0x00,
0x04,0x04,0x04,0x07,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
};

unsigned char Hzk1[]=   //汉字字库 "月"
{
0x00,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,0x00,
0x80,0x40,0x30,0x0F,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x82,0x7F,0x00,0x00,0x00,
};

unsigned char Hzk2[]=   //汉字字库 "日"
{
0x00,0x00,0x00,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x00,0x00,0x00,0x00,
};

unsigned char Hzk3[]=   //汉字字库 "度"
{
0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,
};

unsigned char Hzk4[]=   //汉字字库 "温"
{
0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,

0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,
};

unsigned char Hzk5[]=   //汉字字库 "湿"
{
0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,

0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,
};

unsigned char gImage_clock[128] = {     //闹钟logo图形库
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
  CLK_ICKR = 0x01; //Internal clock control register。复位值，使能HSI时钟，快速唤醒禁止
  CLK_ECKR = 0x00; //External clock control register。HSE禁止。
  CLK_SWR = 0xE1; //Clock master switch register。选择HSI为主时钟。
  CLK_SWCR = 0x00; //Clock switch control register。时钟切换，
  CLK_CKDIVR=0x01;    //1分频
  CLK_PCKENR1 = 0xFF; //Peripheral clock gating register 1。Fmaster与外设时钟连接
  CLK_PCKENR2 = 0x0; //同上
  CLK_CSSR = 0x0; //Clock security system register。时钟安全检测关闭
  CLK_CCOR = 0x0; //Configurable clock control register。时钟输出关闭
}

void TimeShow(void)//时间显示功能
{
  OLED_ShowNum(18,0,DS1302_Time[0],2,16);//年
  OLED_ShowNum(55,0,DS1302_Time[1],2,16);//月
  OLED_ShowNum(95,0,DS1302_Time[2],2,16);//日
  OLED_ShowNum(0,2,DS1302_Time[3],2,16);//时
  OLED_ShowNum(25,2,DS1302_Time[4],2,16);//分
  OLED_ShowNum(50,2,DS1302_Time[5],2,16);//秒
  OLED_ShowNum(120,2,DS1302_Time[6],1,16);//星期
}

void DHTShow(void)//温湿度数据的显示
{
  OLED_ShowNum(0, 6, temperature_u, 2, 16);
  OLED_ShowNum(22, 6, temperature_f, 1, 16);
  OLED_ShowNum(80, 6, humidity_u, 2, 16);
  OLED_ShowNum(102, 6, humidity_f, 1, 16);
}

void TimeSet(void)//时间设置功能
{
  if(KeyNum == 2)       //选中下一个时间变量
  {
      TimeSetSelect++;
      TimeSetSelect%=7;   //DS1302_Time数组的index,在0~6内切换
  }
  if(KeyNum == 3)       //将我们选中的变量加1
  {
    DS1302_Temp[TimeSetSelect]++;       
    
    //下面主要针对时间的取值范围进行限制
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
  if(KeyNum==4)         //将我们选中的变量减1
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
  
  //如果我们选中了一个时间,那么对其进行闪烁标识
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

void ClockSet(void){    //闹钟设置功能
  static uint8_t clockSelcet = 0;
  
  if(KeyNum == 2)       //选中下一个闹钟时间
  {
      clockSelcet++;
      clockSelcet%=2;   //clockSelcet数组的index,在3,4内切换
  }
  
  if(KeyNum == 3)       //选中的时间加1
  {
    DS1302_Temp[clockSelcet + 3]++;
    if(DS1302_Temp[3]>23){DS1302_Temp[3]=0;}
    if(DS1302_Temp[4]>59){DS1302_Temp[4]=0;}
  }
  
  if(KeyNum == 4)       //选中的时间减1
  {
    DS1302_Temp[clockSelcet + 3]--;
    if((signed char)DS1302_Temp[3]<0){DS1302_Temp[3]=23;}
    if((signed char)DS1302_Temp[4]<0){DS1302_Temp[4]=59;}
  }
  
  //如果我们选中了一个时间,那么对其进行闪烁标识
  if(clockSelcet==0 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(0,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(0,2,DS1302_Temp[3],2,16);}
  
  if(clockSelcet==1 && TimeSetFlashFlag==1)     {OLED12864_ShowChianese(25,2,Hzk,sizeof(Hzk));}
  else {OLED_ShowNum(25,2,DS1302_Temp[4],2,16);}
}

void  timer_Init(void)
{
  TIM1_PSCRH = 0;
  TIM1_PSCRL = 7;          //(7+1)分频为1M
  TIM1_ARRH = 0x0;
  TIM1_ARRL = 0x16;        //每10ms中断一次
  TIM1_IER = 0x01;         //允许更新中断
  TIM1_CR1 = 0x01;         //计数器使能，开始计数
}

void main(void)
{   
  asm("sim");    //关总中断
  
  Fosc_Init();        //时钟分频配置
  timer_Init();       //定时中断配置
  LED_init();         //LED配置
  Key_Init();         //按键初始化配置
  BEEP_init();        //蜂鸣器初始化配置
  DS1302_Init();      //DS1302部分配置
  DHT11_Init();       //温湿度传感器配置
  OLED_Init();        //OLED配置
  
  OLED_Clear();       //清屏
  //DS1302_SetTime();   //设置时间,配置一次即可,芯片会自动走时
  
  OLED_ShowNum(1,0,20,2,16);
  OLED12864_ShowChianese(35,0,Hzk0,sizeof(Hzk0));     //年
  OLED12864_ShowChianese(75,0,Hzk1,sizeof(Hzk1));     //月
  OLED12864_ShowChianese(115,0,Hzk2,sizeof(Hzk2));    //日
  OLED12864_ShowChianese(17,2,Hzk3,sizeof(Hzk3));     //度
  OLED12864_ShowChianese(43,2,Hzk3,sizeof(Hzk3));     //度
  OLED_ShowString(80,2,"WEEK:");
  OLED_ShowString(16,6,".");          //温度的小数点
  OLED_ShowString(96,6,".");          //湿度的小数点
  OLED12864_ShowChianese(0,4,Hzk4,sizeof(Hzk4));      //温
  OLED12864_ShowChianese(80,4,Hzk5,sizeof(Hzk5));     //湿
  OLED_ShowChar(114,6,'%',16);
  OLED_ShowClock32x32Logo(42, 4, gImage_clock);       //闹钟模式

  asm("rim");    //开总中断

  while(1)
  {
  }
}

void clock_task(uint8_t *beepFlag, uint16_t *targetDelay){
  static uint16_t timeCounter; //每10ms加一
  timeCounter++;
  
  if( DS1302_Time[3] == clock_hour && DS1302_Time[4] == clock_min){
    switch(*targetDelay){
    
    case 200:   //2秒关机
      if(timeCounter <= *targetDelay){
        BEEP_stop();
        *beepFlag = 0;
      }else{
        timeCounter = 0;
        *targetDelay = 500;
      }
      break;
    
    case 500:   //5秒蜂鸣
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
    case 0 :        //正常显示模式
      for(int i = 0; i< 7; i++){
        DS1302_Temp[i] = DS1302_Time[i];
      }
      MODE = 1;
      TimeSetSelect=0;
      break;
    case 1:         //时间配置模式
      MODE = 2;
      break;
    case 2:         //闹钟配置模式
      MODE = 0;     
      break;
    default:
      MODE = 0;
      break;
    }			
  }
 
  if(KeyNum == 5 && MODE == 1){ //使时间配置生效
    DS1302_configTime(); 
  }  
  
  if(KeyNum == 5 && MODE == 2){ //使闹钟配置生效
    clock_hour = DS1302_Temp[3];
    clock_min = DS1302_Temp[4];
    MODE = 0;
  }
  
  if(KeyNum == 6){  //切换闹钟开关,若闹铃正在响起,则关闭当前闹铃.
    BEEP_stop(); 
    CLOCK_MODE = !CLOCK_MODE;
    
    if(CLOCK_MODE == 1){        //若开启了闹铃,则显示闹钟logo,反之不显示
      OLED_ShowClock32x32Logo(42, 4, gImage_clock);
    }else{
      uint8_t clear[128] = {0x00};
      OLED_ShowClock32x32Logo(42, 4, clear);
    }
  }  
}

#pragma   vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_OVR_UIF(void)     //10ms进入一次定时器中断
{
    TIM1_SR1_UIF = 0;   //清除中断标识
    static uint8_t timCount = 0;
    static uint16_t reFreshCount = 0;
    static uint16_t beepDelay = 500;

    timCount++;
    reFreshCount++;

    key_task();       //定时处理按键任务
    
    if(CLOCK_MODE == 1) {
      clock_task(&BEEP_STATUS, &beepDelay);
    }

    if(timCount > 5){   //定时更新传感器数据
      timCount = 0;
      DS1302_ReadTime();
      DHT11_Read_Data(&temperature_u,&humidity_u,&temperature_f,&humidity_f);
    }
    
    if(reFreshCount >= 50){     //定时刷新OLED屏幕
      reFreshCount = 0;
      
      LED_Toggle();     // LED闪烁
      
      TimeSetFlashFlag = !TimeSetFlashFlag;
      
      DHTShow();
      
      switch(MODE)
      {
      case 0:TimeShow(); break;  //正常显示模式
      case 1:TimeSet();break;   //时间配置模式
      case 2:ClockSet();break; //闹钟配置模式
      default: TimeShow(); break;           
      }
    }    
}
