#include "oledUi.h"

Ui::Ui(Oled *_oled, std::vector<UiTemplateEntry> _entries)
    : oled(_oled)
{
    for (size_t i = 0; i < _entries.size(); i++) {
        blocks.push_back(UiBlock(_entries[i].block,_entries[i].x,_entries[i].y));
    }
    SetOled(oled);
}

Ui::Ui(std::vector<UiTemplateEntry> _entries)
{
    for (size_t i = 0; i < _entries.size(); i++) {
        blocks.push_back(UiBlock(_entries[i].block,_entries[i].x,_entries[i].y));
    }
}

void Ui::SetOled(Oled *oled) {
    for (size_t i = 0; i < blocks.size(); i++) {
        blocks[i].SetOled(oled);
    }
}
