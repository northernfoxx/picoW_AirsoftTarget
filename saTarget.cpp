#include "saTarget.h"
#include <stdint.h>
#include <Arduino.h>


Target::Target(uint8_t sPin, uint8_t lPin, String name){
  name = name;
  sensorPin = sPin;
  ledPin = lPin;
  hitTime = millis();
  gpTimer = millis();
  targetHit = false;
  m_pHitFunc = &Target::ledOff;
  

  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);

}

void Target::ledOn(){
  digitalWrite(ledPin, HIGH);
}
void Target::ledOff(){
  digitalWrite(ledPin, LOW);
}
void Target::timerStart(){
  gpTimer = millis();
}
void Target::ChangeHitAction(uint8_t state){
  if (state==0){
    m_pHitFunc = &Target::ledOn;
  }
  else if(state==1){
    m_pHitFunc = &Target::ledOff;
  }
}
/*
void Target::timerStop(){
  
}*/
int Target::timerDiffMs(){
  return millis() - gpTimer;
}

void Target::updateStatus(){
  if( targetHit ){
    // Access the member function
    (this->*m_pHitFunc)();
    targetHit = false;
  }
}
