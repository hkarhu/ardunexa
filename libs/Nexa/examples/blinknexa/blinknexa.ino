/*
  NEXA radio controlled wall socket remote controller.
    
  Demo to show how to connect remote socket and blink it.
  See Nexa.h and Nexa.cpp for more thorough information.
   
  Code by
   Irah
   tsanio
   2/2016
*/

#include <Nexa.h>

Nexa nexa;

// the setup routine runs once when you press reset:
void setup() {    

    // Connect to correct pin in Arduino for broadcasting singal.
    nexa.setRadioPin(2);    
    
    // Set 10 seconds broadcasting for time consuming processes ie. connecting new device or clearing devices.
    nexa.setLongConfigurationWaitTimeSeconds(10); 

}

// To only introduce device once.
boolean introduced=false;

// Ie coffee maker 0 .. n
int dev = 0; 

void loop() {
  
  // Connect devices ie coffee maker, light... Re-plug the socket right after starting arduino app.
  if (!introduced) {
    nexa.introduceReceiver(dev);
    introduced=true;
  }
    
  // Blink states of individual device.
  delay(3000); nexa.setReceiverState(dev, false); delay(3000); nexa.setReceiverState(dev, true);    
  
  // Blink all.
  delay(3000); nexa.setGlobalState(false); delay(3000); nexa.setGlobalState(true);
  
}

