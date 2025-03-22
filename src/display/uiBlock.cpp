#include "oledUi.h"

UiBlock::UiBlock(const UiBlock &block, uint8_t x, uint8_t y)
    : x(x),
    y(y)
{
    Serial.println("UiBlock::UiBlock");

    for(size_t i = 0; i < block.tfList.size(); i++) {
        Serial.print("UiBlock::UiBlock::for ");
        Serial.println(i);
        tfList.push_back(TextField(block.tfList[i],x,y));
        Serial.print("DONE UiBlock::UiBlock::for ");
        Serial.println(i);
        
    }

}

void UiBlock::SetOled(Oled *oled) {
    for (size_t i = 0; i < tfList.size(); i++) {
        tfList[i].SetOled(oled);
    }
}