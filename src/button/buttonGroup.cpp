#include "buttonGroup.h"

ButtonGroup::ButtonGroup(std::vector<std::shared_ptr<Button>> _buttons) :
    buttons(_buttons)
{}

void ButtonGroup::Spin(unsigned long ts) {
    for (auto &b : buttons) {
        b->Spin(ts);
    }
}

std::optional<std::shared_ptr<Button>>
ButtonGroup::getButton(uint8_t id) {
    for (auto &b : buttons) {
        if (b->getId() == id) {
            return b;
        }
    }
    return std::nullopt;
}

bool ButtonGroup::isPressed(uint8_t id) {
    auto maybeB = getButton(id);
    if (!maybeB) return false;
    auto b = *maybeB;

    return b->btn_state == PRESSED || b->btn_state == HOLD;
}

void ButtonGroup::assignInteractiveObject(ButtonInteractive* obj) {
    for (auto &b : buttons) {
        b->assignInteractiveObject(obj);
    }
}