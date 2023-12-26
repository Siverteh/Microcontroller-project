#ifndef lcd_scrolling
#define lcd_scrolling

#include "mbed.h"
#include "declarations.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>

static void lcdScrolling(string strings[], size_t size, bool infinite, uint32_t flag, bool &dataFetched) {
    int screenLength = 16;

    eventFlags.wait_all(flag, osWaitForever, false);

    string message;
    for (int i = 0; i <= size; i++) {
        // Add spaces between the different articles
        for (int j = 0; j < screenLength; j++)
            message += " ";

        message += strings[i];
    }

    while (true) {
        for (int i = 0; i <= message.length() - screenLength; i++) {

            eventFlags.wait_all(flag, osWaitForever, false);

            if (dataFetched) {
                dataFetched = false;
                return;
            }

            string subString = message.substr(i, screenLength);
            lcd.setCursor(0, 1);
            lcd.clear();
            lcd.printf("%s\n", subString.c_str());
            ThisThread::sleep_for(300ms);
        }

        if (!infinite)
            return;
    }
}

#endif