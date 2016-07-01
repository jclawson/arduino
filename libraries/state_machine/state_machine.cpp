#include "state_machine.h"


StateMachine::StateMachine () {
  transitions = new Transition[MAX_STATES];
  states = new State2[MAX_STATES];
  transitionsSize = 0;
  statesSize = 0;
  currentState = 1000;
}

void StateMachine::addState(State2 t) {
  states[t.state] = t;
  
  if (t.state < currentState) {
    // initialize the currentState to the smallest state passed in.
    // TODO: allow setting the initial state
    currentState = t.state;
  }
  // TODO bounds check
}

void StateMachine::addTransition(Transition t) {
  transitions[transitionsSize++] = t;
  // TODO bounds check
}

void StateMachine::run() {
  // Determine if a state transition should occur.
  for (int i = 0; i < transitionsSize; i++) {
    if (transitions[i].from == currentState) {
      if (transitions[i].shouldTransition()) {
        int newState = transitions[i].to;
        states[currentState].runLoop(END);
        states[newState].runLoop(START);
        currentState = newState;
        return;
      }
    }
  }

  // run the current state loop
  states[currentState].runLoop(LOOP);
}

