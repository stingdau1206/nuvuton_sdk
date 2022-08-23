#ifndef MY_IR_H_
#define MY_IR_H_

#define IR_OFF 0x027E
#define IR_ON_SPEED 0x027D
#define IR_MODE 0x027B
#define IR_TIME 0x0277
#define IR_SWING 0x026F

#include "MS51_16K.H"

void IR_Init(void);
uint8_t IR_Check(uint32_t *pu32Cmd);

#endif