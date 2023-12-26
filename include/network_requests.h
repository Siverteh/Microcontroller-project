#ifndef network_requests
#define network_requests

#include "include/declarations.h"
#include <cstdlib>
#include <json.hpp>
#include <string>

using json = nlohmann::json;

nsapi_size_or_error_t sendRequest(Socket &socket, const char *request);

nsapi_size_or_error_t readResponse(Socket &socket, char *buffer, int bufferLength);

json getJsonRequest(const char *host, const char request[]);

#endif