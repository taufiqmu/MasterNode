#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#define LED_BUILTIN 2
#define SENSOR  12

extern unsigned long totalMilliLitres;

void Flow_Setup(void);
void Flow_Read(void);

#endif