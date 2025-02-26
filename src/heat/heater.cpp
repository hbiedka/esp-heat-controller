#include "heater.h"

Heater::Heater(uint8_t _pin, bool *_input) {
    pin = _pin;
    input = _input;
    pinMode(pin,OUTPUT);
    digitalWrite(pin,0);

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