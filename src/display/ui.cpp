#include "oledUi.h"

Ui::Ui(Oled *_oled, std::vector<UiTemplateEntry> _entries)
    : oled(_oled)
{
    for (size_t i = 0; i < _entries.size(); i++) {
        Serial.print("Ui::Ui::for ");
        Serial.println(i);
        blocks.push_back(UiBlock(_entries[i].block,_entries[i].x,_entries[i].y));
        blocks[i].SetOled(oled);
        Serial.print("DONE Ui::Ui::for ");
        Serial.println(i);
    }
}