# ardunexa :zap:
NEXA remote socket controller for Arduino 

Contains library and example programs.
See ref-folder how to do soldering of remote controller in order to connect it into Arduino.

##Quick How-to use

First install library into your environment. (In Windows Arduino IDE: just copy it into libraries-folder)<br />
<br />
**Make instance.**<br />
Nexa nexa;<br />
<br />
**Set output pin.**<br />
void setup() {<br />
&nbsp;nexa.setRadioPin(2);<br />
}<br />
<br />
**Introduce new device.**<br />
nexa.introduceReceiver(dev);<br />
<br />
**Set state of remote socket.**<br />
nexa.setReceiverState(dev, false);<br />
<br />
Open one of the example projects to see more complete usage.