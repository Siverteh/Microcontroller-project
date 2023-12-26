#ifndef clockFunctions
#define clockFunctions

#include "mbed.h"
#include "include/declarations.h"
#include <cstdio>
#include <ctime>
#include <string>
#include <time.h>

static void startClock() {
    set_time(startTime);
}

static void setClockTime(time_t time) {
    set_time(time);
}

static time_t getCurrentDateTime() {
    return time(NULL);
}

static std::string formatDateTime(time_t time) {
    char buffer[32];
    strftime(buffer, 32, "%a %d %b %H:%M:%S", localtime(&time));
    std::string result = buffer;

    return result;
}

static std::string getCurrentFormatedDateTime() {
    time_t currentTime = getCurrentDateTime();
    std::string result = formatDateTime(currentTime);

    return result;
}

static std::string formatClockTime(time_t time) {
    char buffer[32];
    strftime(buffer, 32, "%a %d %b %H:%M", localtime(&time));
    std::string result = buffer;

    return result;
}

static std::string getCurrentFormatedClockTime() {
    time_t currentTime = getCurrentDateTime();
    std::string result = formatClockTime(currentTime);

    return result;
}

static time_t getCurrentDate() {
    time_t currentTime = getCurrentDateTime();
    struct tm *tmInfo;
    tmInfo = localtime(&currentTime);
    tmInfo->tm_sec = tmInfo->tm_min = tmInfo->tm_hour = 0;
    return mktime(tmInfo);
}

static std::string formatDate(time_t time) {
    char buffer[32];
    strftime(buffer, 32, "%a %d %b", localtime(&time));
    std::string result = buffer;

    return result;
}

static std::string getCurrentFormatedDate() {
    time_t currentDate = getCurrentDate();
    std::string result = formatDate(currentDate);
    printf("%s\n", result.c_str());
    return result;
}

#endif