#include "rLadderButton.h"

bool RLadderButton::GetState() {
    auto inputP = input.lock();
    if (!inputP) return false;

    unsigned int state = inputP->getState();
    bool pressed = (state > thr_low && state < thr_high);
    return pressed;
}
