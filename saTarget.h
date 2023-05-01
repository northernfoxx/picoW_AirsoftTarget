#ifndef _SATARGET_H_
#define _SATARGET_H_
#include <stdint.h>
#include <Arduino.h>

class Target{
  public:
  String name;
  uint8_t sensorPin;
  uint8_t ledPin;
  uint32_t hitTime;
  uint32_t gpTimer;
  bool targetHit;
  //void (*hitFunc)(void);

  Target(uint8_t sPin, uint8_t lPin, String name);
  void ledOn();
  void ledOff();
  void timerStart();
  //void timerStop();
  int timerDiffMs();
  void updateStatus();
  void ChangeHitAction(uint8_t state);

  private:
  typedef void (Target::*TARGET_FUNC)(void);
  TARGET_FUNC m_pHitFunc;
};

typedef void(Target::*MEMBER_FUNC)(void); 
#endif