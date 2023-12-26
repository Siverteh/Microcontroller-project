#include "include/network_requests.h"
#include <cstdio>

// Function to send request
nsapi_size_or_error_t sendRequest(Socket &socket, const char *request) 
{
    nsapi_size_t bytesToSend = strlen(request);
    nsapi_size_or_error_t bytesSent = 0;

    printf("Sending message:\n-----------------------------------------------------------------------------\n%s", request);
    printf("-----------------------------------------------------------------------------\n");

    // Loop until the whole message is sent
    while (bytesToSend) 
    {
        // Tries to send remaining data, returns amount of bytes left
        bytesSent = socket.send(request + bytesSent, bytesToSend);

        if (bytesSent < 0) 
            return bytesSent; 
        else 
            printf("Sent %d bytes\n", bytesSent);

        // Updates bytesToSend to the new amount of remaining bytes.
        bytesToSend -= bytesSent;
    }
    printf("Complete message sent\n");
    return 0;
}

// Function to read response
nsapi_size_or_error_t readResponse(Socket &socket, char *buffer, int bufferLength) {
    int remainingBytes = bufferLength;
    int receievedBytes = 0;

    // Loops until the whole message is read (in case we don't read the whole message in one call)
    while (remainingBytes > 0) 
    {
        nsapi_size_or_error_t result = socket.recv(buffer + receievedBytes, remainingBytes);

        // Break if all bytes are read
        if (result == 0) 
            break;
        // Return if there are errors (negative value)
        if (result < 0) 
            return result;

        receievedBytes += result;
        remainingBytes -= result;
    }

    printf("\nReceieved %d bytes:\n%.*s\n\n", receievedBytes, strstr(buffer, "\n") - buffer, buffer);

    return receievedBytes;
}

json getJsonRequest(const char *host, const char request[]) {
    // Variable to store result from requests
    nsapi_size_or_error_t result;
    char jsonSource[2000] = {0};
    
    while (true)
    {
        socket.open(network);

        result = network->gethostbyname(host, &socket_address);

        if (result != 0) {
            printf("Failed to get IP address of host: %s %d\n", host, result);
            socket.close();
            continue;
        }
        else
            printf("IP address of server %s is %s\n", host, socket_address.get_ip_address());

        socket_address.set_port(80);

        result = socket.connect(socket_address);

        if (result != 0) {
            printf("Failed to connect to server at %s: %d\n", host, result);
            socket.close();
            continue;
        }
        else
            printf("Sucessfully connected to server %s\n", host);

        result = sendRequest(socket, request);

        if (result != 0) {
            printf("Failed to send request: %d\n", result);
            socket.close();
            continue;
        }

        result = readResponse(socket, jsonSource, 2000);

        if (result < 0) {
            printf("Failed to read response: %d\n", result);
            socket.close();
            continue;
        }

        socket.close();
        break;
    }

    char *jsonSourceStart = strchr(jsonSource, '{');

    if (jsonSourceStart == nullptr) {
        printf("Could not find JSON data!\n");
        return nullptr;
    }
    // Ensures final character is '}'
    int jsonSourceSize = strlen(jsonSource);
    while (jsonSourceSize > 0)
    {
        if (jsonSource[jsonSourceSize] == '}')
            break;
        else
            jsonSource[jsonSourceSize] = '\0';

        jsonSourceSize -= 1;
    }
    if ((jsonSourceSize == 0) || (jsonSource + jsonSourceSize < jsonSourceStart))
    {
        printf("No ending \"}\" found, invalid JSON\n");
        return nullptr;
    }

    json jsonData = json::parse(jsonSourceStart, nullptr, false);

    if (jsonData.is_discarded())
    {
        printf("The input JSON data is not valid!\n");
        return nullptr;
    }
    else {
        printf("The input JSON data is valid!\n");
        return jsonData;
    }
}