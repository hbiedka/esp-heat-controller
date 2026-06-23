#include "button.h"

void Button::CommitState(bool s) {
    last_pressed = s;
}

void Button::Spin(unsigned long ts) {

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

                if (press_cb != nullptr) press_cb(id);
                if (unified_cb != nullptr) unified_cb(id,false);
                if (interactiveObject != nullptr) interactiveObject->ButtonCallback(id,false);

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

                    if (hold_cb != nullptr) hold_cb(id);
                    if (unified_cb != nullptr) unified_cb(id,true);
                    if (interactiveObject != nullptr) interactiveObject->ButtonCallback(id,true);
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
                    if (hold_cb != nullptr) hold_cb(id);
                    if (unified_cb != nullptr) unified_cb(id,true);
                    if (interactiveObject != nullptr) interactiveObject->ButtonCallback(id,true);
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


void Button::assignInteractiveObject(ButtonInteractive* obj) {
    interactiveObject = obj;
}