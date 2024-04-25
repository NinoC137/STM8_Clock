#include "key.h"

#define         KEY1    PA_IDR_IDR3     //ģʽѡȡ  0 -- ��ʾ 1 -- ����
#define         KEY2    PF_IDR_IDR4     //����������Ч: next ��һ������
#define         KEY3    PB_IDR_IDR7     //����������Ч: +     ѡ�е���,��1
#define         KEY4    PB_IDR_IDR6     //����������Ч: -     ѡ�е���,��1
#define         KEY5    PC_IDR_IDR3     //ȷ��
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