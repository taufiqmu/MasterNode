#ifndef SCHEDULE_WATERING_H
#define SCHEDULE_WATERING_H

extern int waktu[6];
extern uint16_t totalPlants;

void Schedule_Setup(void);
void Schedule_Loop(void);

#endif