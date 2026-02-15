#include "buttonGroup.h"

void ButtonGroup::Spin(unsigned long ts) {
    for (auto &b : buttons) {
        b.Spin(ts);
    }
}

void ButtonGroup::setCallbackForId(uint8_t id, void (*cb)(uint8_t)) {
    for (auto &b : buttons) {
        if (b.getId() == id) {
            b.setPressCb(cb);
            break;
        }
    }
}

void ButtonGroup::setCallbackForAll(void (*cb)(uint8_t)) {
    for (auto &b : buttons) {
        b.setPressCb(cb);
    }
}

void ButtonGroup::setHoldCallbackForId(uint8_t id, void (*cb)(uint8_t)) {
    for (auto &b : buttons) {
        if (b.getId() == id) {
            b.setHoldCb(cb);
            break;
        }
    }
}

void ButtonGroup::setHoldCallbackForAll(void (*cb)(uint8_t)) {
    for (auto &b : buttons) {
        b.setHoldCb(cb);
    }
}

void ButtonGroup::setUnifiedCallbackForId(uint8_t id, void (*cb)(uint8_t,bool)) {
    for (auto &b : buttons) {
        if (b.getId() == id) {
            b.setCb(cb);
            break;
        }
    }
}

void ButtonGroup::setUnifiedCallbackForAll(void (*cb)(uint8_t,bool)) {
    for (auto &b : buttons) {
        b.setCb(cb);
    }
}