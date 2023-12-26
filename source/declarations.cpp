#include "include/declarations.h"
#include <string>

// Buttons
InterruptIn button0(PC_5, PullDown);
InterruptIn button1(PC_4, PullDown);
InterruptIn button2(PC_3, PullDown);
InterruptIn button3(PC_2, PullDown);

// LEDs
DigitalOut ledDefault(LED1);

// LCD screen
DFRobot_RGBLCD lcd(16, 2, D14, D15);

// Alarm
PwmOut signal(PB_1);

// Network
TCPSocket socket;
Mutex socketMutex;
SocketAddress socket_address;
NetworkInterface *network = NetworkInterface::get_default_instance();

// Configuring I2C for sensors
DevI2C i2c(PB_11, PB_10);
HTS221Sensor sensor(&i2c);

// Initialize objects
void initDeclarations() {
    lcd.init();
    sensor.init(NULL);
    sensor.enable();
}

// Variables
time_t startTime = 0;
int currentScreen = 0;
float temperature = 0.0;
float humidity = 0.0;

// Event flags
EventFlags eventFlags;

// Variables alarm
std::string alarmclock;
bool alarmIsSet = false;
int hour = 0;
int minute = 0;
//Temporary variables used in case user does not complete alarm
int tempHour = 0;
int tempMinute = 0;

int counter = 0;
bool alarmIsRinging = false;
bool alarmIsOn = false;
bool setHours = true;
bool setMinutes = true;

float internetTemperature = 0.0;
// Counts upwards and resets to 0 to prevent API-call spamming
int refreshWeather = 0;
int refreshNews = 0;
// Counts upwards to 4 to allow information on which screen we're currently viewing to be displayed first
int infoScreenTimer = 0;

bool newsFethced = false;

std::string cloud = "";

// Stores top three news articles
string firstArticle = { 0 };
string secondArticle = { 0 };
string thirdArticle = { 0 };
