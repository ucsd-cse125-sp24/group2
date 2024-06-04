#include "States.hpp"

States::States() {
  for(int i = 0; i < LAST_STATE; i++)
  {
      states[static_cast<ObjectStates>(i)] = false;
  }
}

bool States::checkState(ObjectStates state) const {
    return states.at(state);
}

void States::disableState(ObjectStates state) {
    states[state] = false;
}

void States::enableState(ObjectStates state) {
    states[state] = true;
}