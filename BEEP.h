#ifndef __BEEP_H
#define __BEEP_H

#include <iostm8s103k3.h>

#define uint8_t   unsigned char
#define uint16_t  unsigned int
#define uint32_t  unsigned long

#define BEEP_OUT PD_ODR_ODR4 

extern uint8_t BEEP_STATUS;
extern uint8_t CLOCK_MODE;

void BEEP_init(void);

void BEEP_start(void);

void BEEP_stop(void);

#endif