#include "button.h"

bool RLadderButton::GetState() {
    //TODO move to cpp file
    if (state == nullptr) return false;

    bool pressed = (*state > thr_low && *state < thr_high);
    return pressed;
}

void RLadderButton::CommitState(bool s) {
    last_pressed = s;
}

void RLadderButton::Spin(unsigned long ts) {
    //TODO move to cpp file

    bool pressed = GetState();

    switch(btn_state) {
        case RELEASED:
            if (pressed && !last_pressed) {
                btn_state = WAIT_FOR_PRESS;
                state_ts = ts;
            }
            break;
        case WAIT_FOR_PRESS:
            if (pressed && (state_ts + t_press) < ts) {
                btn_state = PRESSED;
                //TODO some press action
                if (press_cb != nullptr) press_cb(id);
                state_ts = ts;  //reset state_ts to wait to hold
                
            } else if (!pressed) {
                btn_state = RELEASED;
            }
            break;
        case PRESSED:
            if (pressed) {
                if ((state_ts + t_hold) < ts) {
                    btn_state = HOLD;
                    state_ts = ts;
                    t_hold_buffer = t_hold;

                    //TODO some hold action
                    if (hold_cb != nullptr) hold_cb(id);
                }
            } else {
                btn_state = RELEASED;
            }
            break;
        case HOLD:
            if (pressed) {
                if ((state_ts + t_hold_buffer) < ts) {
                    if (t_hold_buffer > t_hold_sustain) {
                        t_hold_buffer -= t_hold_decay;
                    }
                    state_ts = ts;
                    //TODO some hold action
                    if (hold_cb != nullptr) hold_cb(id);
                }
            } else {
                btn_state = RELEASED;
            }
            break;
        default:
            break;
    }

    CommitState(pressed);

};

void RLadderButton::setPressCb(void (*cb)(uint8_t)) {
    press_cb = cb;
}

void RLadderButton::setHoldCb(void (*cb)(uint8_t)) {
    hold_cb = cb;
}
