#ifndef declarations
#define declarations

#include "mbed.h"
#include "libs/DFRobot_RGBLCD.h"
#include "libs/HTS221/HTS221Sensor.h"
#include <string>

// Buttons
extern InterruptIn button0;
extern InterruptIn button1;
extern InterruptIn button2;
extern InterruptIn button3;

// LEDs
extern DigitalOut ledDefault;

// LCD screen
extern DFRobot_RGBLCD lcd;

// Alarm
extern PwmOut signal;

// Network
extern TCPSocket socket;
extern Mutex socketMutex;
extern SocketAddress socket_address;
extern NetworkInterface *network;

// Configuring I2C for sensors
extern DevI2C i2c;
extern HTS221Sensor sensor;

// Initialize objects
void initDeclarations();

// Variables
extern time_t startTime;
extern time_t currentTime;
extern int currentScreen;
extern float temperature;
extern float humidity;
extern float internetTemperature;
extern int refreshWeather;
extern int refreshNews;
extern int infoScreenTimer;
extern bool newsFethced;

extern std::string cloud;

// Variables alarm
extern std::string alarmclock;
extern bool alarmIsSet;
extern int hour;
extern int minute;
extern int tempHour;
extern int tempMinute;
extern int counter;
extern bool alarmIsRinging;
extern bool alarmIsOn;
extern bool setHours;
extern bool setMinutes;

// Stores top three news articles
extern string firstArticle;
extern string secondArticle;
extern string thirdArticle;


// Flags
extern EventFlags eventFlags;
#define NETWORK_FLAG (1 << 0)
#define UNIX_EPOCH_TIME_FLAG (1 << 1)
#define NEWS_FLAG (1 << 2)
#define LOADING_WAIT_FLAGS (NETWORK_FLAG | UNIX_EPOCH_TIME_FLAG)
#define DISPLAY_UNIX_EPOCH_TIME_FLAG (1 << 3)
#define NEWS_SCROLLING_FLAG (1 << 4)

#endif