#ifndef OLED_DISPLAY_H
#define OLEH_DISPLAY_H

#include <Arduino.h>

#define __DEBUG__ (0)
#define TFT_RST 14
#define TFT_CS 33
#define TFT_DC 26

extern int moist, soil, lumen, temp;
extern bool pumpControl, autoControl, relayState;

const uint8_t STATUSBUTTON  = 34;
const uint8_t PUMPBUTTON    = 32;
const uint8_t AUTOBUTTON    = 35;
const uint8_t ENTERBUTTON   = 25;
const uint8_t COILPIN       = 27;

void OledRun(void);
void OledSetup(void);
void OledData(void);
String httpGETRequest(const char* serverName);
String httpGETRequest(const char* serverName);

#endif