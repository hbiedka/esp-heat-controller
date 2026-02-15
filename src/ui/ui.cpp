#include "ui.h"

Ui::Ui(Oled *_oled, const std::vector<UiTemplateEntry> &entries)
    : oled(_oled)
{
    loadTemplate(entries);
}

Ui::Ui(Oled *_oled)
    : oled(_oled)
{}

Ui::Ui(const std::vector<UiTemplateEntry> &entries)
{
    loadTemplate(entries);
}

Ui::Ui()
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