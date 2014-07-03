#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Pro or Pro Mini (5V, 16 MHz) w/ ATmega328
#define __AVR_ATmega328P__
#define 
#define _VMDEBUG 1
#define ARDUINO 101
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void printMenu();
void off();
void allLEDS(unsigned long color);
void blink();
void calibrate();
void cascade(unsigned long color, byte direction, byte wait);
void clearpixel();
void colorFlush();
void colorWipe(uint32_t c, uint8_t wait);
void cylon(unsigned long color, byte wait);
void cylon2();
void firstLight();
void rainbow(uint8_t wait);
void rainbow2(byte startPosition);
void rainbowCycle(uint8_t wait);
uint32_t rainbowOrder(byte position);
void siren();
uint32_t Wheel(byte WheelPos);
void dimmer();
void dimmer1();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Vault\FTCL\FTCL.ino"
#include "D:\Vault\FTCL\Functions.ino"
#endif
