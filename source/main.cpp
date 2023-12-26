#include "mbed.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "include/declarations.h"
#include "source/screens.cpp"
#include "source/boot.cpp"

int main()
{
    // Initialize components, variables and such
    initDeclarations();

    // This thread handles the bootup screen and other stuff that needs to be started on boot
    // Porgram waits until thread is finished to continue
    Thread bootThread;
    bootThread.start(bootUp);
    bootThread.join();

    // This thread handles displaying and swtitching between the different screens
    Thread screenThread;
    screenThread.start(displayScreens);
    screenThread.set_priority(osPriorityRealtime7);
    screenThread.join();
}