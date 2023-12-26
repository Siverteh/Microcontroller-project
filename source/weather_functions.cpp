#include <cstdlib>
#include <json.hpp>
#include "include/declarations.h"
#include "include/network_requests.h"


using json = nlohmann::json;

void getWeatherData() {
    const char *host = "api.weatherapi.com";

    // We hva decided to hardcode Grimstad into the API request, as there is no reliable way for us to get the smart clocks location.
    // The closest we could get would be to use the IP Geolocation, but this can have very mixed results.
    // ex. when using a network with Uninett as the Internett service provider, your location according to IP Geolocation services would be Trondheim
    // A solution to this problem would be to use a GPS, but this microcontroller does not have one.
    const char request[] = "GET /v1/current.json?key=5699082827554b858a1184745222005&q=Grimstad&aqi=yes HTTP/1.1\r\n"
                            "Host: api.weatherapi.com\r\n"
                            "Connection: close\r\n"
                            "\r\n";

    printf("\nGetting weather data...\n");

    socketMutex.lock();    
    json response = getJsonRequest(host, request);
    socketMutex.unlock();

    // Updates the temperature
    if (response["current"]["temp_c"].is_number_float())
        internetTemperature = response["current"]["temp_c"];
    
    // Updates the weather condition
    if (response["current"]["condition"]["text"].is_string())
        cloud = response["current"]["condition"]["text"];

    printf("\nReceieved the following:\n");
    printf("Temperature: %.1f\n", internetTemperature);
    printf("Forecast: %s\n", cloud.c_str());

    return;
    
}