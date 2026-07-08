#include "ui.h"

Ui::Ui(Oled &_oled,  ButtonGroup &_bg, const std::vector<UiTemplateEntry> &entries)
    : Ui(_oled,_bg)
{
    loadTemplate(entries);
}

Ui::Ui(Oled &_oled, ButtonGroup &_bg)
    : oled(_oled),
    bg(_bg)
{}

void Ui::loadTemplate(const std::vector<UiTemplateEntry> &entries)
{
    blocks.clear();
    for (size_t i = 0; i < entries.size(); i++) {
        blocks.push_back(UiBlock(entries[i].block,entries[i].x,entries[i].y));
        blocks.back().SetOled(&oled);
    }
}

void Ui::Clear()
{
    oled.Clear();
}

void Ui::takeoverButtons(ButtonInteractive* obj)
{
    bg.assignInteractiveObject(obj);
}

bool Ui::OledWakeUpPulse() {
    return oled.WakeUpPulse();
}