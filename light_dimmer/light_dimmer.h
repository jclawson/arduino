#ifndef _LIGHT_DIMMER_H_
#define _LIGHT_DIMMER_H_

const int thinkingLedPin = 13; // this pin will blink when the lights are fading, on when they are on, and off when they are off.
const int ledPin = 5;      // the pin that the LED is attached to
const long fadeInterval = 50;
const int lightMax = 255; // maximum LED PWM value
const int lightInc = 1; //LED PWM incremental value
const int noMotionExpirationMs = 5000; // 10s
const int motionPin = 2;

enum State {
  OFF,
  FADE_UP,
  ON,
  FADE_DOWN
};

void fadeUpLightsLoop(TransitionPhase phase);
void fadeDownLightsLoop(TransitionPhase phase);
bool isStateOn();
bool isMotionDetected();
bool isStateOff();
bool isNoMotionTimeExpired();
void noOpLoop(TransitionPhase phase){
  digitalWrite(thinkingLedPin, !isMotionDetected() ? LOW : HIGH);
};


#endif
