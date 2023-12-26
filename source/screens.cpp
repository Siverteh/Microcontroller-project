#ifndef screens
#define screens

#include "mbed.h"
#include "include/declarations.h"
#include "include/set_alarm.h"
#include "include/weather_functions.h"
#include "include/news_feed.h"
#include "source/clock.cpp"
#include <cstdio>

static int maxScreens = 3;

static void nextScreen() {
    currentScreen++;
    infoScreenTimer = 0;
    refreshWeather = 0;
    refreshNews = 0;
    eventFlags.clear(NEWS_SCROLLING_FLAG);

    //Modified to allow screen 4. 
    //5 was chosen so a double click would not trigger a jump from 2 to that screen.
    if (currentScreen > maxScreens && currentScreen != 5) {
        currentScreen = 0;
    }
}

static void previousScreen() {
    currentScreen--;
    infoScreenTimer = 0;
    refreshWeather = 0;
    refreshNews = 0;
    eventFlags.clear(NEWS_SCROLLING_FLAG);

    if (currentScreen < 0) {
        currentScreen = maxScreens;
    }
}

static void displayScreens() {
    // Start news thread
    Thread newsThread;
    newsThread.start(fetchNewsFeed);
    bool newsThreadStarted = false;

    Thread newsScrollingThread;
    newsScrollingThread.start(displayNewsFeed);

    // Set button 0 and 2 to change between screens
    while (true) {
        //Set between 1 and -1, so alarm doesn't trigger when switched from off to on.
        //alarmIsOn as an argument, so it doesn't trigger when toggled off
        //alarmIsSet so it doesn't trigger at midnight
        if (remainingSecondsUntilAlarm() < 1 && remainingSecondsUntilAlarm() > -1 && alarmIsOn && alarmIsSet)
        {
            alarmIsRinging = true;
        }

        if (!alarmIsRinging)
            {   
                //Remapping of the buttons after alarm screen
                if (button0.read())
                {
                    previousScreen();
                }

                if (button1.read() && currentScreen == 0)
                {
                    goToAlarmScreen();
                }
                
                if (button2.read())
                {
                    nextScreen();
                }

                if (button3.read())
                {
                    turnOffAlarm();
                }
            }
        
        else if (alarmIsRinging)
        {
            alarmSound();

            if (button0.read())
            {
                continue;
            }

            if (button1.read())
            {
                snooze();
            }

            if (button2.read())
            {
                continue;
            }

            if (button3.read())
            {
                turnOffSiren();
            }
        } 
        

        switch (currentScreen) {
            case 0:     // CLOCK
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.printf(getCurrentFormatedClockTime().c_str());

                //If the alarm is on and set, the alarm will be displayed as Alarm h:m
                if (alarmIsOn && alarmIsSet)
                {
                    lcd.setCursor(0, 1);
                    lcd.printf("Alarm     %s", alarmclock.c_str());
                }

                //If the alarm is not on, but set, the alarm will be displayed as Alarm OFF h:m
                else if (!alarmIsOn && alarmIsSet)
                {
                    lcd.setCursor(0, 1);
                    lcd.printf("Alarm OFF %s", alarmclock.c_str());
                }
                
                break;
            }

            case 1:     // LOCAL TEMP AND HUMIDITY
            {
                if (infoScreenTimer < 3)
                {
                    lcd.setCursor(0, 0);
                    lcd.clear();
                    lcd.printf("Local room");
                    lcd.setCursor(0, 1);
                    lcd.printf("information");
                    infoScreenTimer++;
                }
                else 
                {
                    sensor.get_temperature(&temperature);
                    sensor.get_humidity(&humidity);
                    lcd.setCursor(0, 0);
                    lcd.clear();
                    lcd.printf("Temp: %.1f C", temperature);
                    lcd.setCursor(0,1);
                    lcd.printf("Humidity: %.0f%%", humidity);
                }
                break;
            }

            case 2:     // WEATHER FORECAST
            {
                // Prevents unneccecary spamming to weather API
                if (refreshWeather == 0)
                {
                    getWeatherData();
                    refreshWeather++;
                }  
                else if (refreshWeather < 3600)
                    refreshWeather++;
                else
                    refreshWeather = 0;
                
                if (infoScreenTimer < 3)
                {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.printf("Weather");
                    lcd.setCursor(0, 1);
                    lcd.printf("forecast");
                    infoScreenTimer++;
                }
                else
                {
                    lcd.setCursor(0, 0);
                    lcd.clear();
                    lcd.printf("%.1f degrees", internetTemperature);
                    lcd.setCursor(0,1);
                    lcd.printf("%s", cloud.c_str());
                }
                break;
            }

            case 3:     // NEWS SCREEN
            {
                if (infoScreenTimer < 3)
                {
                    lcd.setCursor(0, 0);
                    lcd.clear();
                    lcd.printf("Top");
                    lcd.setCursor(0, 1);
                    lcd.printf("news!");
                    infoScreenTimer++;
                }
                else
                {
                    eventFlags.set(NEWS_SCROLLING_FLAG);
                }
                break;
            }
            
            case 4:     // SET ALARM
            {
                setAlarm();
                break;
            }
        }

        
        ThisThread::sleep_for(500ms);
    }
}

#endif