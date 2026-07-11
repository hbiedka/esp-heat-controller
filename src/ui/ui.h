#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "display/oled.h"
#include "display/oledUi.h"
#include "button/buttonGroup.h"

class Ui {
    private:
        std::vector<std::vector<TextField>> blocks;
        Oled &oled;
        ButtonGroup &bg;

    public:
        Ui(Oled &_oled, ButtonGroup &_bg, const std::vector<UiTemplateEntry> &entries);
        Ui(Oled &_oled, ButtonGroup &_bg);
        void loadTemplate(const std::vector<UiTemplateEntry> &entries);
        void takeoverButtons(ButtonInteractive* obj);
        void Clear();
        bool OledWakeUpPulse();
        std::vector<TextField> &operator[](size_t i) { return blocks[i]; };
        unsigned int getBlocksNum() { return blocks.size(); };
};

#endif