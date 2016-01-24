/*
  NEXA Radio controlled wall socket remote controller.
  by Irah
 */

// NEXA
//[Unknown 26 bits][Global select bit][ON/OFF bit][4 bits Device select]

int radio = 2;
//                   0 0 1 0 1 0 0 0 0 0 1 1 1 0 0 0 0 1 0 1 0 0 1 1 1 0 1 1 0 0 0 0 //Channel Global ON
//                   0 0 1 0 1 0 0 0 0 0 1 1 1 0 0 0 0 1 0 1 0 0 1 1 1 0 0 1 1 1 0 0 //Channel 4/1 ON
//                   0 0 1 0 1 0 0 0 0 0 1 1 1 0 0 0 0 1 0 1 0 0 1 1 1 0 0 0 1 1 0 0 //Channel 4/1 OFF
                   
char dataON[32] = {0,0,1,0,1,0,0,0,0,0,1,1,1,0,0,0,0,1,0,1,0,0,1,1,1,0,0,1,0,0,1,1}; //Channel 1/1 ON
char dataOFF[32] = {0,0,1,0,1,0,0,0,0,0,1,1,1,0,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,1,1}; //Channel 1/1 OFF

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(radio, OUTPUT);     
}

void loop() {
  pulseON();
  delay(2000);
  pulseOFF();
  delay(2000);
}

void pulseON(){
  for(int n=0; n<4; n++){
    //Starting pulse
    digitalWrite(radio, HIGH);
    delayMicroseconds(265);
    digitalWrite(radio, LOW);
    delayMicroseconds(2650);
    
    for(char i=0; i < 32; i++){
      digitalWrite(radio, HIGH);
      delayMicroseconds(265);
      digitalWrite(radio, LOW);
        
      if(dataON[i] == 0){
        delayMicroseconds(265);
        digitalWrite(radio, HIGH);
        delayMicroseconds(265);
        digitalWrite(radio, LOW);
        delayMicroseconds(1260);
      } else {
        delayMicroseconds(1260);
        digitalWrite(radio, HIGH);
        delayMicroseconds(265);
        digitalWrite(radio, LOW);
        delayMicroseconds(265);
      }
    }
    
    digitalWrite(radio, HIGH);
    delayMicroseconds(265);
    digitalWrite(radio, LOW);
    
    delay(10);
  }
}

void pulseOFF(){
  for(int n=0; n<4; n++){ 
    //Starting pulse
    digitalWrite(radio, HIGH);
    delayMicroseconds(265);
    digitalWrite(radio, LOW);
    delayMicroseconds(2650);
    
    for(char i=0; i < 32; i++){
      digitalWrite(radio, HIGH);
      delayMicroseconds(265);
      digitalWrite(radio, LOW);
        
      if(dataOFF[i] == 0){
        delayMicroseconds(265);
        digitalWrite(radio, HIGH);
        delayMicroseconds(265);
        digitalWrite(radio, LOW);
        delayMicroseconds(1260);
      } else {
        delayMicroseconds(1260);
        digitalWrite(radio, HIGH);
        delayMicroseconds(265);
        digitalWrite(radio, LOW);
        delayMicroseconds(265);
      }
    }
    
    digitalWrite(radio, HIGH);
    delayMicroseconds(265);
    digitalWrite(radio, LOW);
    
    delay(10);
  }
}
