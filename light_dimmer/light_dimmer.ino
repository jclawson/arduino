#include "../state_machine/state_machine.h"
#include "light_dimmer.h"

// State variables
// =================================================
int currentBrightness = 0;
bool motionDetected = false;
State currentState = OFF;
unsigned long currentTimeMillis;
unsigned long motionLastSeenAtMillis;
// ==================================================

/**
 * Returns true when the time since that last time motion was seen has exceeded the noMotionExpirationMs.
 */
bool isNoMotionTimeExpired() {
  return (currentTimeMillis - motionLastSeenAtMillis) > noMotionExpirationMs;
}

/**
 * Returns true when the LED state should be ON.
 */
bool isStateOn() {
  return currentBrightness >= lightMax;
}

/**
 * Returns true when the current currentBrightness is 0, and teh state should be OFF.
 */
bool isStateOff() {
  return currentBrightness <= 0; 
}

/**
 * Returns true when motion was detected;
 */
bool isMotionDetected() {
  return motionDetected;
}

/**
 * Helper function that fades the LED up or down by inc.
 */
void fadeLights(bool fadeUp) {
  static unsigned long previousMillis = 0;
  static int ledState = LOW;
  
  if ((previousMillis + fadeInterval) < currentTimeMillis) {

    ledState = !ledState;
    digitalWrite(thinkingLedPin, ledState);
    
    int inc = (fadeUp) 
        ? lightInc 
        : (-1 * lightInc);
    currentBrightness = max(0, min(currentBrightness + inc, lightMax));
    analogWrite(ledPin, currentBrightness);
    previousMillis = currentTimeMillis;
  }
}

/**
 * Inrements the LED brightness by one increment
 */
void fadeUpLightsLoop(TransitionPhase phase) {
  fadeLights(true);
}

/**
 * Decrements the LED brightness by one increment
 */
void fadeDownLightsLoop(TransitionPhase phase) {
  fadeLights(false);
}


void readMotionState() {
  int motionVal = digitalRead(motionPin);
  if (motionVal == HIGH) {
    motionDetected = true;
  } else {
    motionDetected = false;
  }
  

  if (motionDetected) {
    motionLastSeenAtMillis = currentTimeMillis;
  }
}


StateMachine stateMachine;

void setup() {
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(motionPin, INPUT); 

  stateMachine.addState({OFF,       &noOpLoop});
  stateMachine.addState({FADE_UP,   &fadeUpLightsLoop});
  stateMachine.addState({ON,        &noOpLoop});
  stateMachine.addState({FADE_DOWN, &fadeDownLightsLoop});
  
  stateMachine.addTransition({OFF,       FADE_UP,   &isMotionDetected});
  stateMachine.addTransition({FADE_UP,   ON,        &isStateOn});
  stateMachine.addTransition({ON,        FADE_DOWN, &isNoMotionTimeExpired});
  stateMachine.addTransition({FADE_DOWN, FADE_UP,   &isMotionDetected});
  stateMachine.addTransition({FADE_DOWN, OFF,       &isStateOff});
}

void loop() {
  // initialize the current time in millis for this event loop iteration
  currentTimeMillis = millis();

  // initialize the motion detection state for this iteration of the loop
  readMotionState();

  stateMachine.run();
}
