#include "ui.h"

Ui::Ui(Oled *_oled,  ButtonGroup *_bg, const std::vector<UiTemplateEntry> &entries)
    : oled(_oled),
    bg(_bg)
{
    loadTemplate(entries);
}

Ui::Ui(Oled *_oled, ButtonGroup *_bg)
    : oled(_oled),
    bg(_bg)
{}

void Ui::loadTemplate(const std::vector<UiTemplateEntry> &entries)
{
    blocks.clear();
    for (size_t i = 0; i < entries.size(); i++) {
        blocks.push_back(UiBlock(entries[i].block,entries[i].x,entries[i].y));
        blocks.back().SetOled(oled);
    }
}

void Ui::Clear()
{
    if (!oled) return;  //TODO throw exception
    oled->Clear();
}

void Ui::takeoverButtons(ButtonInteractive* obj)
{
    if (!bg) return;    //TODO throw exception
    bg->assignInteractiveObject(obj);
}