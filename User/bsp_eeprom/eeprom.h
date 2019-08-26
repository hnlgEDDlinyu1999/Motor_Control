#ifndef _EEPROM_H
#define _EEPROM_H

#include "./bsp_I2c/i2c.h"

uint8_t E2ReadByte(uint8_t addr);
void E2WriteByte(uint8_t addr, uint8_t dat);
void E2Read(uint8_t *buf, uint8_t addr, int len);
void E2Write(const uint8_t *buf, uint8_t addr, int len);

#endif /*_EEPROM_H*/
/***********************************THE END************************************/