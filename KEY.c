#include "key.h"

#define         KEY1    PA_IDR_IDR3     //模式选取  0 -- 显示 1 -- 设置
#define         KEY2    PF_IDR_IDR4     //在配置中生效: next 下一项配置
#define         KEY3    PB_IDR_IDR7     //在配置中生效: +     选中的项,加1
#define         KEY4    PB_IDR_IDR6     //在配置中生效: -     选中的项,减1
#define         KEY5    PC_IDR_IDR3     //确认
#define         KEY6    PC_IDR_IDR2

void  delay_ms(uint8_t  ms)
{
    uint8_t  i,j;
    while(ms--)
    {
        for(i=4;i!=0;i--)
          for(j=10;j!=0;j--);
    }
}

void Key_Init(void)
{
    //KEY1配置
    PA_DDR_DDR3 = 0;            //PA3输入模式
    PA_CR1_C13 = 1;             //上拉输入
    PA_CR2_C23 = 0;             //禁止外部中断
    //KEY2配置
    PF_DDR_DDR4 = 0;            //PF4输入模式
    PF_CR1_C14 = 1;             //上拉输入
    PF_CR2_C24 = 0;             //禁止外部中断
    //KEY3配置
    PB_DDR_DDR7 = 0;            //PB7输入模式
    PB_CR1_C17 = 1;             //上拉输入
    PB_CR2_C27 = 0;             //禁止外部中断
    //KEY4配置
    PB_DDR_DDR6 = 0;            //PB6输入模式
    PB_CR1_C16 = 1;             //上拉输入
    PB_CR2_C26 = 0;             //禁止外部中断
    //KEY5配置
    PC_DDR_DDR3 = 0;            //PC3输入模式
    PC_CR1_C13 = 1;             //上拉输入
    PC_CR2_C23 = 0;             //禁止外部中断
    //KEY6配置
    PC_DDR_DDR2 = 0;            //PC2输入模式
    PC_CR1_C12 = 1;             //上拉输入
    PC_CR2_C22 = 0;             //禁止外部中断
}

uint8_t KeyScan(void)       //
{
    uint8_t KeyNumber=0;
    
    if(KEY1 == 0){delay_ms(10);while(KEY1 == 0);delay_ms(10);KeyNumber = 1;}
    if(KEY2 == 0){delay_ms(10);while(KEY2 == 0);delay_ms(10);KeyNumber = 2;}
    if(KEY3 == 0){delay_ms(10);while(KEY3 == 0);delay_ms(10);KeyNumber = 3;}
    if(KEY4 == 0){delay_ms(10);while(KEY4 == 0);delay_ms(10);KeyNumber = 4;}
    if(KEY5 == 0){delay_ms(10);while(KEY5 == 0);delay_ms(10);KeyNumber = 5;}
    if(KEY6 == 0){delay_ms(10);while(KEY6 == 0);delay_ms(10);KeyNumber = 6;}
    
    return KeyNumber;
}