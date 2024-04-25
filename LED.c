#include "LED.h"

void LED_init(void){
    //LED配置
    PE_DDR_DDR5 = 0;            //PE5输出模式
    PE_CR1_C15 = 0;             //开漏输出
    PE_CR2_C25 = 0;             //低速
}

void LED_Toggle(void){
  LED_OUT = !LED_OUT;
}