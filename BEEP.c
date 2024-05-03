#include "BEEP.h"

uint8_t BEEP_STATUS = 0;        //0为关闭,1为开启
uint8_t CLOCK_MODE = 1;         //0为关闭,1为开启

void BEEP_init(void){
    //蜂鸣器配置
    PD_DDR_DDR4 = 1;            //PD4输出模式
    PD_CR1_C14 = 1;             //推挽输出
    PD_CR2_C24 = 0;             //低速
}

void BEEP_start(void){
  BEEP_STATUS = 1;
  BEEP_OUT = 1;
}

void BEEP_stop(void){
  BEEP_OUT = 0;
  BEEP_STATUS = 0;
}