#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#define MAX_STATES 20

#include <stdlib.h>
#include <assert.h>

enum TransitionPhase {
  START, // passed the first time a state is entered
  LOOP,  // passed on each loop a state is in effect
  END    // passed when the state has transitioned to a new state (before calling START on the new state loop)
};

struct Transition {
   int  from;
   int  to;
   bool  (*shouldTransition)();
};

struct State2 {
  int state;
  void  (*runLoop)(TransitionPhase);
};

class StateMachine {

  int currentState = 0;
  Transition* transitions;
  int transitionsSize = 0;

  State2* states;
  int statesSize = 0;
  
  public:
    StateMachine ();
    void addTransition(Transition);
    void addState(State2);
    void run();
};

#endif
