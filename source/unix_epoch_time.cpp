#ifndef unix_epoch_time
#define unix_epoch_time

#include "mbed.h"
#include <cstdio>
#include <include/declarations.h>
#include <json.hpp>
#include <string>
#include "include/network_requests.h"
#include "source/clock.cpp"

using json = nlohmann::json;

static void fetchUnixEpochTime() {
    const char *host = "worldtimeapi.org";

    const char request[] = "GET /api/ip HTTP/1.1\r\n"
                            "Host: worldtimeapi.org\r\n"
                            "Connection: close\r\n"
                            "\r\n";

    printf("\nGetting Unix Epoch Time...\n");

    socketMutex.lock();
    json response = getJsonRequest(host, request);
    socketMutex.unlock();

    time_t unixEpochTime = 0;
    time_t timezoneOffset = 0;
    time_t dstOffset = 0;

    // Updates the clock
    if (response["unixtime"].is_number_integer())
        unixEpochTime = response["unixtime"];

    if (response["raw_offset"].is_number_integer() && !response["raw_offset"].is_null())
        timezoneOffset = response["raw_offset"];

    if (response["dst_offset"].is_number_integer() && !response["dst_offset"].is_null())
        dstOffset = response["dst_offset"];

    setClockTime(unixEpochTime + timezoneOffset + dstOffset);

    printf("\n");
    printf("Receieved the following:\n");
    printf("Timestamp: %i\n", unixEpochTime);
    printf("timezoneOffset: %i\n", timezoneOffset);
    printf("dstOffset: %i\n", dstOffset);
    printf("\n");

    eventFlags.set(UNIX_EPOCH_TIME_FLAG);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("Unix epoch time:");
    lcd.setCursor(0, 1);
    lcd.printf("%i", unixEpochTime);
    ThisThread::sleep_for(5s);
    
    eventFlags.set(DISPLAY_UNIX_EPOCH_TIME_FLAG);
    return;
}

#endif