#include "oledUi.h"

UiBlock::UiBlock(const UiBlock &block, uint8_t x, uint8_t y)
    : x(x),
    y(y)
{
    for(size_t i = 0; i < block.tfList.size(); i++) {
        tfList.push_back(TextField(block.tfList[i],x,y));
    }

}

void UiBlock::SetOled(Oled *oled) {
    for (size_t i = 0; i < tfList.size(); i++) {
        tfList[i].SetOled(oled);
    }
}