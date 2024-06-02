#ifndef STATES_HPP
#define STATES_HPP

#include <map>

enum ObjectStates {
    VISIBLE,
    LAST_STATE,
};


class States {
public:
    States();
    bool checkState(ObjectStates state) const;
    void disableState(ObjectStates state);
    void enableState(ObjectStates state);
private:
    std::map<ObjectStates, bool> states;
};

#endif 