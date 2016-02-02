#include <math.h>
#include <string.h>
#include "Arduino.h"
#include "Nexa.h"

/**
* A library to control NEXA radio controlled wall sockets.
*
* Can handle 0 .. n sockets. The receiver (remote controlled socket) must be linked to the system using introduceReceiver(n)
* Receiver can be cleared from old senders using clearReceiverCompletely(n)
* 
* Code by
*  Irah
*  tsanio 
*  2/2016
*/

Nexa::Nexa() {  
	senderBase                    = 10543438; // Number < 67mil
	radioPin                      = 2;  // Output pin
	maxReceivers                  = 3;	// This will automatically increase in case more devices are connected.
	debug                         = false;
	longConfigurationWaitTimeSec  = 10; // Time to broadcast some messages ie to introduce new device and to let user do some actions ie. re-plug socket.
	pinMode(radioPin, OUTPUT);		
}

// NEXA message
// [Sender 26 bits][Global select bit][ON/OFF bit][4 bits Device select]
// 0 0 1 0 1 0 0 0 0 0 1 1 1 0 0 0 0 1 0 1 0 0 1 1 1 0 1 1 0 0 0 0 //Channel Global ON
// 0 0 1 0 1 0 0 0 0 0 1 1 1 0 0 0 0 1 0 1 0 0 1 1 1 0 0 1 1 1 0 0 //Channel 4/1 ON
// 0 0 1 0 1 0 0 0 0 0 1 1 1 0 0 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 //Channel 4/1 OFF

// Can be linked to handle state of 0 - n Nexa switches.

// Broadcasts the given sequence to devices.
void Nexa::sendPulseSequence(boolean *sequence, int len){
  for(int n=0; n<4; n++){ 
    //Starting pulse
    digitalWrite(radioPin, HIGH);
    delayMicroseconds(265);
    digitalWrite(radioPin, LOW);
    delayMicroseconds(2650);
    
    for(char i=0; i < len; i++){
      digitalWrite(radioPin, HIGH);
      delayMicroseconds(265);
      digitalWrite(radioPin, LOW);
        
      if(sequence[i] == false){
        delayMicroseconds(265);
        digitalWrite(radioPin, HIGH);
        delayMicroseconds(265);
        digitalWrite(radioPin, LOW);
        delayMicroseconds(1260);
      } else {
        delayMicroseconds(1260);
        digitalWrite(radioPin, HIGH);
        delayMicroseconds(265);
        digitalWrite(radioPin, LOW);
        delayMicroseconds(265);
      }
    }
    
    digitalWrite(radioPin, HIGH);
    delayMicroseconds(265);
    digitalWrite(radioPin, LOW);
    
    delay(10);
  }
}

// number to binary representation.
boolean* Nexa::pBinFill(long x, boolean *b, int width)
{ // fill in array from right to left
	int  i=width-1;
	do
	{
		b[i--]=(x & 1) ? true : false;
		x>>=1;  // shift right 1 bit
	} while( x > 0);
	while(i>=0) b[i--]=false;    // fill with fillChar 
	return b;
}


void Nexa::setRadioPin(int pin) {
	radioPin = pin;
	pinMode(radioPin, OUTPUT);
}

int Nexa::getRadioPin() {
	return radioPin;	
}

void Nexa::setSenderBase(long senderBase_) {
	senderBase = senderBase_;
}

long Nexa::getSenderBase() {
	return senderBase;
}

void Nexa::setLongConfigurationWaitTimeSeconds(int longConfigurationWaitTimeSec_) { longConfigurationWaitTimeSec = longConfigurationWaitTimeSec_; }
		
int Nexa::getLongConfigurationWaitTimeSeconds() { return longConfigurationWaitTimeSec; }

// Each sender can handle only 3 devices so we just create new sender after each 3 devices.
long Nexa::getSenderForReceiver(int receiver) {
  return senderBase + ((long)( floor((float)receiver)/3.0) );
}

// Returns amount of senders
int Nexa::getAmountOfSenders() {
  int amount = ((int)( floor((float)maxReceivers)/3.0) );
  return amount == 0 ? 1 : amount;
}

// Returns sender 0 .. n
long Nexa::getSender(long n) {
  return senderBase + n;
}



// Run this and plug Nexa to calibrate it into system. (15 sec time to plug)
void Nexa::introduceReceiver(int receiver) {
	sendStateMessage_For_Configuration(getSenderForReceiver(receiver), receiver, false, true);
}

// Clears all senders from Nexa. (15 sec time to plug)
void Nexa::clearReceiver(int receiver) {  
	sendStateMessage_For_Configuration(getSenderForReceiver(receiver), receiver, false, false);
}

// Clears all three senders from Nexa. (15 sec time to plug)
void Nexa::clearReceiverCompletely(int receiver) { 
	// I'm not sure can it be done from sender that is not already in the memory? 
	sendStateMessage_For_Configuration(getSenderForReceiver(receiver), receiver, true, false);
}

// Broadcasts message to all devices.
void Nexa::setGlobalState(boolean state) {	
	for (int i=0;i<getAmountOfSenders();i++)
		sendStateMessage(getSender(i), 0, true, state);
}

// Sets state of device.
void Nexa::setReceiverState(int receiver, boolean state) {
	sendStateMessage(getSenderForReceiver(receiver), receiver, false, state);
}


// Broadcasts the message for some time to make it possible to go connect Nexa into wall.
void Nexa::sendStateMessage_For_Configuration(long sender, int receiver, boolean global, boolean state) {	
	int secs2 = longConfigurationWaitTimeSec / 2;
	if (secs2==0) secs2=1; // interval of 2 secs but input in 1sec ;)
	for (int i=0;i<secs2;i++) {
		sendStateMessage(sender, receiver, global, state);
		delay(2000);
	}
}

void Nexa::sendStateMessage(long sender, int receiver, boolean global, boolean state) {
	

  updateReceiverAmount(receiver);
  
  
  // Get 26 bit representation of the sender.
  boolean* b = new boolean[26];
  boolean* senderSequence = pBinFill(sender, b, 26);

  
  
  // Create sequence. Start by adding sender 26bits.
  boolean *sequence = new boolean[32];
  for (int i=0;i<26;i++) {
      sequence[i] = senderSequence[i];
  }
  
  
  // Global (broadcast to all)
  if (global)
    sequence[26] = true;
  else
    sequence[26] = false;



  // State On/Off
  if (state)
    sequence[27] = true;
  else
    sequence[27] = false;



  // Receiver 1-4 00, 01, 10, 11
  int receiverMod = receiver % 3;
  if (receiverMod==0||receiverMod==1)
    sequence[28] = false;
  else
    sequence[28] = true;
    
  if (receiverMod==0||receiverMod==2)
    sequence[29] = false;
  else
    sequence[29] = true;



  // Last 2 bits ..? Let's just set them 1..
  sequence[30] = false;
  sequence[31] = false;

  
  
  // Debug print code  
  if (debug) {
	String debugStr = "Sending " + String(state) + " to channel " + String(receiver) + " (" + String(receiverMod) + ") sender: ";  
	Serial.print(debugStr.c_str());
	Serial.print(String(sender) + "(");  
	for (int i=0;i<26;i++)
		Serial.print(sequence[i]);
	Serial.println(") Global " + String(global));  
  }
  
  
  
  // Broadcast
  sendPulseSequence(sequence, 32);
  
}

void Nexa::beginDebug(int speed, boolean beginSerial) {
	debug=true;
	if (beginSerial)
		Serial.begin(speed);
}

void Nexa::endDebug(boolean closeSerial) {
	debug=false;
	if (closeSerial)
		Serial.end();
}

void Nexa::setMaxReceiverAmount(int amount) {
	maxReceivers=amount;
}

void Nexa::updateReceiverAmount(int amount) {
	if (amount>maxReceivers)
		setMaxReceiverAmount(amount);
}