#include "mainScreen.h"

void MainScreen::Show() {
    ui.takeoverButtons(this);    
    ui.loadTemplate(uiTemplate);

    ui.Clear();
    ui[0][0].Print("Press OK to enter menu");
}

void MainScreen::buttonEnter() {
    //enter main menu
    Jump(menu);

}

void MainScreen::ButtonCallback(uint8_t id, bool hold) {
    if (id == 2 && !hold) buttonEnter();
}
