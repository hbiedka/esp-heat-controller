#include "heater.h"

Heater::Heater(uint8_t _pin, bool *_input) :
    delaySetterFunctor(*this)
{
    pin = _pin;
    input = _input;
    pinMode(pin,OUTPUT);
    digitalWrite(pin,0);

    omItems ={
        ObjectModelItem{ "name", ObjectModelItemType::STRING, ObjectModelItemValue{std::string{"Heater"}} },
        ObjectModelItem{ "state", ObjectModelItemType::INT, ObjectModelItemValue{0} },
        ObjectModelItem{ "timeToNextState", ObjectModelItemType::INT, ObjectModelItemValue{0} },
        ObjectModelItem{ "on", ObjectModelItemType::BOOL, ObjectModelItemValue{false} },
        ObjectModelItem{ "delayToOn", ObjectModelItemType::INT, ObjectModelItemValue{0}, &delaySetterFunctor },
        ObjectModelItem{ "delayToOff", ObjectModelItemType::INT, ObjectModelItemValue{0}, &delaySetterFunctor }
    };

    firstRun = true;
}

void Heater::Spin(unsigned long ts) {
    if (input == nullptr) return;

    bool in = *input;

    switch(state) {
        case OFF:
            if (in && !last_input) {
                state = DELAY_TO_ON;
                state_ts = ts;
                log("Heater: delay to on");
            }
            break;
        case DELAY_TO_ON:
            if (in && (state_ts + delay_to_on) < ts) {
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
            if (!in && (state_ts + delay_to_off) < ts) {
                state = OFF;
                digitalWrite(pin,0);
                log("Heater: off");
            } else if (in) {
                state = ON;
            }
            break;
    }

    last_input = in;
}

unsigned int Heater::getTimeToOn(unsigned long ts) {
    if (state == DELAY_TO_ON) {
        return (delay_to_on - (ts - state_ts))/1000;
    } else {
        return 0;
    }
}

unsigned int Heater::getTimeToOff(unsigned long ts) {
    if (state == DELAY_TO_OFF) {
        return (delay_to_off - (ts - state_ts))/1000;
    } else {
        return 0;
    }
}

//to be deprecated and replaced by ObjectModel/ some triggers?
bool Heater::watch(unsigned long ts) {
    bool ret = false;
    unsigned int t_to = 0;

    if (firstRun) {
        firstRun = false;
        ret = true;   
    }

    //check times
    if (state == DELAY_TO_ON) t_to = getTimeToOn(ts);
    else if (state == DELAY_TO_OFF) t_to = getTimeToOff(ts);

    if (state != h_data.state) {
        //new state
        ret = true;
    } else if (state == DELAY_TO_ON || state == DELAY_TO_OFF) {
        if (t_to != h_data.timeTo) ret = true;
    }

    if (ret) {
        h_data.state = state;
        h_data.timeTo = t_to;
    }

    return ret;
}

std::vector<ObjectModelItem>& Heater::getObjectModel() {
    auto ts = millis();
    int om_time_to_next_state = 0;

    //calculate cache OM variables
    if (state == DELAY_TO_ON) om_time_to_next_state = getTimeToOn(ts);
    else if (state == DELAY_TO_OFF) om_time_to_next_state = getTimeToOff(ts);

    //update OM values
    omItems[1].value =  static_cast<int>(state);
    omItems[2].value = om_time_to_next_state;
    omItems[3].value = state == ON || state == DELAY_TO_OFF;
    omItems[4].value = static_cast<int>(delay_to_on);
    omItems[5].value = static_cast<int>(delay_to_off);

    return omItems;
}

ObjectModelSetterReturn HeaterDelaySetter::operator()(const std::string &label, const ObjectModelItemValue &value)
{
    if (!std::holds_alternative<int>(value))
        return ObjectModelSetterReturn::INVTYPE;
    
    int ival = std::get<int>(value);
    if (ival < 0 || ival > 1000000)
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
