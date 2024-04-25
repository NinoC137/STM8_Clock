#ifndef __KEY_H
#define __KEY_H

#include <iostm8s103k3.h>

#define         uint8_t   unsigned char
#define         uint16_t    unsigned int
#define         uint32_t   unsigned long

void delay_ms(uint8_t  ms);
void Key_Init(void);

uint8_t KeyScan(void);

#endif
