#include "KEY.h"

void Key_Init(void)
{
    //KEY1����
    PA_DDR_DDR3 = 0;            //PA3����ģʽ
    PA_CR1_C13 = 1;             //��������
    PA_CR2_C23 = 0;             //��ֹ�ⲿ�ж�
    //KEY2����
    PF_DDR_DDR4 = 0;            //PF4����ģʽ
    PF_CR1_C14 = 1;             //��������
    PF_CR2_C24 = 0;             //��ֹ�ⲿ�ж�
    //KEY3����
    PB_DDR_DDR7 = 0;            //PB7����ģʽ
    PB_CR1_C17 = 1;             //��������
    PB_CR2_C27 = 0;             //��ֹ�ⲿ�ж�
    //KEY4����
    PB_DDR_DDR6 = 0;            //PB6����ģʽ
    PB_CR1_C16 = 1;             //��������
    PB_CR2_C26 = 0;             //��ֹ�ⲿ�ж�
    //KEY5����
    PC_DDR_DDR3 = 0;            //PC3����ģʽ
    PC_CR1_C13 = 1;             //��������
    PC_CR2_C23 = 0;             //��ֹ�ⲿ�ж�
    //KEY6����
    PC_DDR_DDR2 = 0;            //PC2����ģʽ
    PC_CR1_C12 = 1;             //��������
    PC_CR2_C22 = 0;             //��ֹ�ⲿ�ж�
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