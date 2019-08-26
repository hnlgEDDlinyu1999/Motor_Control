#ifndef _I2C_H
#define _I2C_H

#include "./delay/delay.h"
#include "./config.h"

void I2CInit(void);
void I2CStart(void);
void I2CStop(void);
uint8_t I2CReadNAK(void);
uint8_t I2CReadACK(void);
uint8_t I2CWrite(uint8_t dat);

#endif /*_I2C_H*/
/***********************************THE END************************************/