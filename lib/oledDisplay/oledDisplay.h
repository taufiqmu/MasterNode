#ifndef OLED_DISPLAY_H
#define OLEH_DISPLAY_H

#include <Arduino.h>

#define TFT_RST 14
#define TFT_CS 33
#define TFT_DC 26

extern int temper, moist, soil, lumen, temp, pres, hum;

void OledRun(void);
void OledSetup(void);
void OledData(void);
String httpGETRequest(const char* serverName);
String httpGETRequest(const char* serverName);

#endif