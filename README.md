# ardunexa
NEXA remote socket controller for Arduino

Contains library and example programs.
See ref-folder how to do soldering for remote controller to connect it into Arduino.

**Quick How-to use**

First install library into your environment. (In Windows Arduino IDE: just copy it into libraries-folder)

#include <Nexa.h>

Nexa nexa;

void setup() {    
    nexa.setRadioPin(2); // Signal out to controller.
}

// Introduce new device.
nexa.introduceReceiver(dev);

// Set state of remote socket.
nexa.setReceiverState(dev, false);

Open one of the example projects to see more complete usage.