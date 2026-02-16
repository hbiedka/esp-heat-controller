#include "heater.h"

Heater::Heater(uint8_t _pin, bool *_input, std::string name) :
    delaySetterFunctor(*this)
{
    pin = _pin;
    input = _input;
    pinMode(pin,OUTPUT);
    digitalWrite(pin,0);

    omItems = {
        {"name", ObjectModelItem{ ObjectModelItemValue{name}}},
        {"state",ObjectModelItem{ ObjectModelItemValue{0} }},
        {"timeToNextState", ObjectModelItem{ ObjectModelItemValue{0} }},
        {"on",ObjectModelItem{ ObjectModelItemValue{false} }},
        {"delayToOn",ObjectModelItem{ ObjectModelItemValue{0}, &delaySetterFunctor }},
        {"delayToOff",ObjectModelItem{ ObjectModelItemValue{0}, &delaySetterFunctor }}
    };

    firstRun = true;
}

void Heater::Spin(unsigned long ts) {
    if (input == nullptr) return;

    bool in = *input;
    int time_to_next_state_ms = 0;
    int time_to_next_state = 0;

    switch(state) {
        case OFF:
            if (in && !last_input) {
                state = DELAY_TO_ON;
                state_ts = ts;
                log("Heater: delay to on");
            }
            break;
        case DELAY_TO_ON:
            time_to_next_state_ms = state_ts + delay_to_on*1000 - ts;
            if (in && time_to_next_state_ms <= 0) {
                state = ON;
                digitalWrite(pin,1);
                log("Heater: on");
            } else if (!in) {
                state = OFF;
            }
            break;
        case ON:
            if (!in) {
                state = DELAY_TO_OFF;
                state_ts = ts;
                log("Heater: delay to off");
            }
            break;
        case DELAY_TO_OFF:
        time_to_next_state_ms = state_ts + delay_to_off*1000 - ts;
            if (!in && time_to_next_state_ms <= 0) {
                state = OFF;
                digitalWrite(pin,0);
                log("Heater: off");
            } else if (in) {
                state = ON;
            }
            break;
    }

    last_input = in;

    time_to_next_state = time_to_next_state_ms / 1000;

    //update Object Model
    updateLocalProperty("state",static_cast<int>(state));
    updateLocalProperty("timeToNextState",time_to_next_state);
    updateLocalProperty("on",state == ON || state == DELAY_TO_OFF);
    updateLocalProperty("delayToOn",static_cast<int>(delay_to_on));
    updateLocalProperty("delayToOff",static_cast<int>(delay_to_off));
}

ObjectModelSetterReturn HeaterDelaySetter::operator()(const std::string &label, const ObjectModelItemValue &value)
{
    if (!std::holds_alternative<int>(value))
        return ObjectModelSetterReturn::INVTYPE;
    
    int ival = std::get<int>(value);
    if (ival < 0 || ival > 1000)
        return ObjectModelSetterReturn::INVVAL;

    if (label == "delayToOn") {
        htr.delay_to_on = ival;
    } else if (label == "delayToOff") {
        htr.delay_to_off = ival;
    } else {
        return ObjectModelSetterReturn::INVLABEL;
    }
    
    return ObjectModelSetterReturn::OK;
}
