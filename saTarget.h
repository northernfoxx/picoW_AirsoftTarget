#ifndef _SATARGET_H_
#define _SATARGET_H_
#include <stdint.h>

class Target{
  public:
  
  uint8_t sensorPin;
  uint8_t ledPin;
  uint32_t hitTime;
  uint32_t gpTimer;
  bool targetHit;
  Target(uint8_t sPin, uint8_t lPin);
  
  //static void hitEvent(void *arg);
  void ledOn();
  void ledOff();
  void updateStatus();
  //void _hitEvent();
};

typedef void(Target::*MEMBER_FUNC)(void); 
#endif