#ifndef __KEY_H
#define __KEY_H

#include <iostm8s103k3.h>

#define uint8_t  unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long

#define KEY1 PA_IDR_IDR3     //模式选取  0 -- 显示 1 -- 设置
#define KEY2 PF_IDR_IDR4     //在配置中生效: next 下一项配置
#define KEY3 PB_IDR_IDR7     //在配置中生效: +     选中的项,加1
#define KEY4 PB_IDR_IDR6     //在配置中生效: -     选中的项,减1
#define KEY5 PC_IDR_IDR3     //确认
#define KEY6 PC_IDR_IDR2     //闹钟状态配置

void Key_Init(void);

uint8_t KeyScan(void);

#endif
