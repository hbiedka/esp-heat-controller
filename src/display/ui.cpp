#include "oledUi.h"

Ui::Ui(Oled *_oled, const std::vector<UiTemplateEntry> &entries)
    : oled(_oled)
{
    loadTemplate(entries);
    SetOled(oled);
}

Ui::Ui(const std::vector<UiTemplateEntry> &entries)
{
    loadTemplate(entries);
}

Ui::Ui()
{    
}

void Ui::SetOled(Oled *oled) {
    for (size_t i = 0; i < blocks.size(); i++) {
        blocks[i].SetOled(oled);
    }
}

void Ui::loadTemplate(const std::vector<UiTemplateEntry> &entries)
{
    blocks.clear();
    for (size_t i = 0; i < entries.size(); i++) {
        blocks.push_back(UiBlock(entries[i].block,entries[i].x,entries[i].y));
    }
}