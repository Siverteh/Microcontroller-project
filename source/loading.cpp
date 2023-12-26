#ifndef loading
#define loading

#include "mbed.h"
#include "declarations.h"
#include "clock.cpp"
#include <cstdio>
#include <string>
#include <time.h>
#include <ctime>

static void loadingScreen() {
    int dots = 0;
    int i = 0;
    while (true) {
        // Exit loading screen when other threads are finished
        
        if (eventFlags.get() == LOADING_WAIT_FLAGS) {
            eventFlags.wait_all(DISPLAY_UNIX_EPOCH_TIME_FLAG);
            return;
        }
        
        // Loading dots
        dots++;
        if (dots > 3) {
            dots = 0;
        }
        
        std::string dotString = "";

        for (int i = 0; i < dots; i++)
            dotString += ".";

        std::string loadingString = "Loading" + dotString;

        // Display on screen
        lcd.clear();

        // Display clock
        lcd.setCursor(0, 0);
        lcd.printf(getCurrentFormatedClockTime().c_str());
        
        // Display loading
        lcd.setCursor(0, 1);
        lcd.printf(loadingString.c_str());

        ThisThread::sleep_for(500ms);
    }
}

#endif