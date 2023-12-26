#include "include/declarations.h"

nsapi_size_or_error_t connectToNetwork()
{
    if (!network)
        {
            printf("The device failed to get the default network interface!\n");
            eventFlags.set(NETWORK_FLAG);
            return NULL;
        }
    
    // Loops until network is connected
    while (true)
    {
        // Connect to network
        nsapi_size_or_error_t result = network->connect();

        // Error checking
        if (result != 0)
        {
            printf("Failed to connect to the network: %d\n", result);
            continue;
        }
        else 
        {
            // Get IP
            result = network ->get_ip_address(&socket_address);
            printf("Connected to the network, got the IP address %s\n", socket_address.get_ip_address());
            eventFlags.set(NETWORK_FLAG);
            return result;
        }
    }    
}