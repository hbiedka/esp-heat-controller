#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include "button.h"
#include <vector>
#include <memory>

class ButtonGroup {
    private:
        std::vector<std::shared_ptr<Button>> buttons;
    public:
        ButtonGroup(std::vector<std::shared_ptr<Button>> _buttons);
        void Spin(unsigned long ts);
        void assignInteractiveObject(ButtonInteractive* obj);
        std::optional<std::shared_ptr<Button>> getButton(uint8_t id);
        bool isPressed(uint8_t id);
};

#endif