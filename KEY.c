#include "KEY.h"

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

uint8_t KeyScan(void)       
{
    uint8_t KeyNum = 0;
    uint8_t tempValue = 0;
    static uint8_t keyValue_old = 0;;
    
    if(KEY1 == 0){
      tempValue = 1;
    }
    if(KEY2 == 0){
      tempValue = 2;
    }
    if(KEY3 == 0){
      tempValue = 3;
    }
    if(KEY4 == 0){
      tempValue = 4;
    }
    if(KEY5 == 0){
      tempValue = 5;
    }
    if(KEY6 == 0){
      tempValue = 6;
    }
    
    if(keyValue_old == tempValue){
      KeyNum = keyValue_old;
      keyValue_old = 0;
    }
    
    keyValue_old = tempValue;
    
    return KeyNum;
}