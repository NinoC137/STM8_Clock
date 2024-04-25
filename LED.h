#ifndef __LED_H
#define __LED_H

#include <iostm8s103k3.h>

#define         uint8_t   unsigned char
#define         uint16_t    unsigned int
#define         uint32_t   unsigned long

#define LED_OUT PB_ODR_ODR5

void LED_init(void);

void LED_Toggle(void);

#endif