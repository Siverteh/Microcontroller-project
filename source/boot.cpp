#ifndef boot
#define boot

#include "mbed.h"
#include "include/declarations.h"
#include "include/network_connectivity.h"
#include "source/loading.cpp"
#include "source/unix_epoch_time.cpp"
#include "include/news_feed.h"
#include <cstdio>
#include <string>

static void bootUp() {
    // Loading
    Thread loadingThread;
    loadingThread.start(loadingScreen);
    
    // Start clock
    startClock();

    // Starts thread to connect to network
    Thread networkThread;
    networkThread.start(connectToNetwork);
    networkThread.join();

    // Start Unix epoch time thread
    Thread unixEpochTimeThread;
    unixEpochTimeThread.start(fetchUnixEpochTime);
    
    // Waits until other threads are finished before continuing
    unixEpochTimeThread.join();
    loadingThread.join();
}

#endif