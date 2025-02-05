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
                
            } else if (!pressed) {
                btn_state = RELEASED;
            }
            break;
        case PRESSED:
            if (!pressed) btn_state = RELEASED;
            break;
        default:
            break;
    }

    CommitState(pressed);

};

void RLadderButton::setPressCb(void (*cb)(uint8_t)) {
    press_cb = cb;
}
