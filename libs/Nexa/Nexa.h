#ifndef Nexa_h
#define Nexa_h

#include "Arduino.h"

// See Nexa.cpp

class Nexa {

	public:
		
		Nexa();
		
		void setRadioPin(int pin);

		int getRadioPin();
		
		void setMaxReceiverAmount(int amount);
		
		void setSenderBase(long senderBase_);
		
		void setLongConfigurationWaitTimeSeconds(int longConfigurationWaitTimeSec_);
		
		int getLongConfigurationWaitTimeSeconds();
		
		long getSenderBase();
		
		void introduceReceiver(int receiver);
		
		void clearReceiver(int receiver);
		
		void clearReceiverCompletely(int receiver);
		
		void setReceiverState(int receiver, boolean state);
		
		void setGlobalState(boolean state);		
		
		void beginDebug(int speed, boolean beginSerial);
		
		void endDebug(boolean closeSerial);
		
	private:
		void sendStateMessage_For_Configuration(long sender, int receiver, boolean global, boolean state);
		void sendStateMessage(long sender, int receiver, boolean global, boolean state);
		boolean* pBinFill(long x, boolean *b, int width);
		void sendPulseSequence(boolean *sequence, int len);
		long getSender(long n);
		int getAmountOfSenders();
		void updateReceiverAmount(int amount);
		long getSenderForReceiver(int receiver);		
				
		int radioPin;
		int longConfigurationWaitTimeSec;
		int maxReceivers;
		long senderBase;
		boolean debug;
};

#endif

