#include "saTarget.h"
#include <stdint.h>
#include <Arduino.h>


Target::Target(uint8_t sPin, uint8_t lPin){
  sensorPin = sPin;
  ledPin = lPin;
  hitTime = millis();
  gpTimer = millis();
  targetHit = false;
  

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);

}
/*
void Target::_hitEvent(){
  if( !targetHit ){
      if((millis() - hitTime) > 100){
          targetHit = true;
          hitTime = millis();
      }
  }
}
*/
/*
static void Target::hitEvent(void *arg){
  Target *tg = static_cast<Target *>(arg);
  if( !tg->targetHit ){
      if((millis() - tg->hitTime) > 100){
          tg->targetHit = true;
          tg->hitTime = millis();
      }
  }
}
*/
void Target::ledOn(){
  digitalWrite(ledPin, HIGH);
}
void Target::ledOff(){
  digitalWrite(ledPin, LOW);
}


void Target::updateStatus(){
  if( targetHit ){
    ledOff();
    targetHit = false;
  }
}
