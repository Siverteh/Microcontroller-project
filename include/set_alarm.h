#ifndef setalarm
#define setalarm

#include "mbed.h"
#include "string.h"
#include <string>
#include "stdio.h"
#include "chrono"
#include "include/declarations.h"


//For comments on functions, see cpp-file

extern void hourPlus();

extern void hourMinus();

extern void minutePlus();

extern void minuteMinus();

extern void counterPlus();

extern void breakAlarm();

extern void setHour();

extern void setMinute();

extern void resetAlarmVariables();

extern void enableOrDisableAlarm();

extern void alarmSound();

extern void soundTheHorns();

extern void snooze();

extern void turnOffAlarm();

extern int alarmInSeconds();

extern int remainingSecondsUntilAlarm();

//For possible implementation into screens.cpp
extern void goToMainScreen();

//For possible implementation into screens.cpp
extern void goToAlarmScreen();

extern void setAlarm();

extern void turnOffSiren();

#endif 