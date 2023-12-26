#ifndef setalarm
#define setalarm

#include "mbed.h"
#include "libs/DFRobot_RGBLCD.h"
#include "libs/json.hpp"
#include "string.h"
#include <string>
#include "stdio.h"
#include "chrono"
#include "include/declarations.h"
#include "clock.cpp"

using namespace chrono;

//Time format when alarm is set
void alarmClockFormat ()
{
    if (hour < 10 && minute < 10)
        alarmclock = "0" + to_string(hour) + ":" + "0"+to_string(minute);
    else if (hour < 10 && minute >= 10)
        alarmclock = "0" + to_string(hour) + ":" + to_string(minute);
    else if (hour >= 10 && minute < 10)
        alarmclock = to_string(hour) + ":" + "0" + to_string(minute);
    else 
        alarmclock = to_string(hour) + ":" + to_string(minute);
}

//Time format when alarm is being set
void tempAlarmClockFormat()
{
    if (tempHour < 10 && tempMinute < 10)
        alarmclock = "Alarm    0" + to_string(tempHour) + ":" + "0" + to_string(tempMinute);
    else if (tempHour < 10 && tempMinute >= 10)
        alarmclock = "Alarm    0" + to_string(tempHour) + ":" + to_string(tempMinute);
    else if (tempHour >= 10 && tempMinute < 10)
        alarmclock = "Alarm    " + to_string(tempHour) + ":" + "0" + to_string(tempMinute);
    else 
        alarmclock = "Alarm    " + to_string(tempHour) + ":" + to_string(tempMinute);
}

// For possible implementation into screens.cpp
void goToMainScreen()
{
    currentScreen = 0;
    counter = 0;
    alarmClockFormat();
}

// For possible implementation into screens.cpp
void goToAlarmScreen()
{
    currentScreen = 4;
}

//Adds one hour when setting the alarm
void hourPlus()
{
    if (tempHour >= 23)
    {
        tempHour = 0;
    }

    else
    {
        tempHour++;
    } 
}

//Subtracts one hour when setting the alarm
void hourMinus()
{
    if (tempHour <= 0)
    {
        tempHour = 23;
    }
    else 
    {
        tempHour--;
    }
}

//Adds one minute when setting the alarm
void minutePlus()
{
    if (tempMinute >= 59)
    {
        tempMinute = 0;
    }

    else
    {
        tempMinute++;
    } 
}

//Substracts one minute when setting the alarm
void minuteMinus()
{   
    if (tempMinute <= 0)
    {
        tempMinute = 59;
    }
    else 
    {
        tempMinute--;
    }
}

//Iterates a counter to keep track of whether hour or minute is being set in alarm.
//If counter == 0, hour is being set, if counter == 1 minute is being set.
//If counter == 2, the alarm is set.
void counterPlus()
{
    counter++;
}

//Button mapping for changing hour when setting alarm
void setHour()
{
        if (button0.read())
        {
            hourMinus();
        }

        else if (button1.read())
        {
            counterPlus();
        }

        else if (button2.read())
        {
            hourPlus();
        }

        else if (button3.read())
        {
            goToMainScreen();
        }
}

//Button mapping for changing minute when setting alarm
void setMinute()
{
    if (button0.read())
        {
            minuteMinus();
        }

    else if (button1.read())
        {
            counterPlus();
        }

    else if (button2.read())
        {
            minutePlus();
        }

    else if (button3.read())
        {
            goToMainScreen();
        }
}

//Enables or disables the alarm
void enableOrDisableAlarm()
{
    alarmIsSet = !alarmIsSet;
}

//Contains the alarm sound
void alarmSound()
{
signal.period(0.003822);
signal.write(0.8);
signal.resume();
}

//Converts the set alarm time to seconds.
int alarmInSeconds()
{
    int seconds = minute*60 + hour*3600;
    
    return seconds;
}

//Checks remaining time until alarm. For potential troubleshooting
int remainingSecondsUntilAlarm()
{
    int seconds = alarmInSeconds();

    //Placeholder variable for current time in seconds. 
    //By using the rest of a modulus 86400 (24 hours in seconds)
    //We can find the amount of seconds so far today, and use that 
    //To check against the set alarm. 

    int secondsToday = getCurrentDateTime() - getCurrentDate();

    int remaining = seconds - secondsToday;   
    return remaining;
}

//Function for snoozing the alarm by 5 minutes
void snooze()
{
    minute += 5;
    alarmIsRinging = false;
    signal.suspend();
    alarmClockFormat();
}

//Turns off the alarm
void turnOffAlarm()
{
    alarmIsOn = !alarmIsOn;
}

void turnOffSiren()
{
    alarmIsRinging = false;
    signal.suspend();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("Alarm stopped");
    ThisThread::sleep_for(2s);
}

void hourScreen()
{
    if (setHours == true)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.printf("Set hour");
        ThisThread::sleep_for(1s);
        setHours = false;
    }
}

void minuteScreen()
{
    if (setMinutes == true)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.printf("Set minute");
        ThisThread::sleep_for(1s);
        setMinutes = false;
    }
}

void setAlarm()
{
    tempHour = hour;
    tempMinute = minute;

//Argument is set to currenctScreen != 0, because currentScreen will change to 0
//if the user presses button 3 or sets the alarm
 while (currentScreen != 0)
    {
        ThisThread::sleep_for(300ms);

        lcd.clear();
        tempAlarmClockFormat();
        lcd.printf(alarmclock.c_str());

        switch (counter)
        {
        case 0:
            hourScreen();
            setHour();
            break;
                
        case 1:
            minuteScreen();
            setMinute();
            break;

        case 2: 
            hour = tempHour;
            minute = tempMinute;
            setHours = true;
            setMinutes = true;
            alarmIsSet = true;
            alarmIsOn = true;
            goToMainScreen();
            break;
        }
    }
}
#endif 