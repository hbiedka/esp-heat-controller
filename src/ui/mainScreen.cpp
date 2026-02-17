#include "mainScreen.h"
#include "../heat/heater.h"

void HeaterStateWatcher::operator()(const std::string &property, const ObjectModelItem &item)
{
    if (!host) return;  //TODO throw exception
    if (!std::holds_alternative<int>(item.value)) return; //TODO throw exception
    host->ShowState(heaterId,std::get<int>(item.value));
};

void HeaterTimeToNextStateWatcher::operator()(const std::string &property, const ObjectModelItem &item)
{
    if (!host) return;  //TODO throw exception
    if (!std::holds_alternative<int>(item.value)) return; //TODO throw exception
    host->ShowTimeToNextState(heaterId,std::get<int>(item.value));
};

MainScreen::MainScreen(Ui &_ui, NavigativeScreen *_menu,ObjectModel *_pump1,ObjectModel *_pump2,ObjectModel *_htr):
    ui(_ui),
    menu(_menu),
    pump1(_pump1),
    pump2(_pump2),
    htr(_htr)
{
    pump1->AddWatcher("state",new HeaterStateWatcher(this,1));
    pump2->AddWatcher("state",new HeaterStateWatcher(this,2));
    htr->AddWatcher("state",new HeaterStateWatcher(this,3));
    pump1->AddWatcher("timeToNextState",new HeaterTimeToNextStateWatcher(this,1));
    pump2->AddWatcher("timeToNextState",new HeaterTimeToNextStateWatcher(this,2));
    htr->AddWatcher("timeToNextState",new HeaterTimeToNextStateWatcher(this,3));
}

void MainScreen::Show() {
    shown = true;
    ui.takeoverButtons(this);    
    ui.loadTemplate(uiTemplate);

    ui.Clear();
    ui[0][0].Print("Press OK to enter menu");
    ui[1][0].Print("Pump 1:");
    ui[2][0].Print("Pump 2:");
    ui[3][0].Print("Heater:");

    int htr_state, p1_state, p2_state;
    pump1->getInt("state",p1_state);
    pump2->getInt("state",p2_state);
    htr->getInt("state",htr_state);

    ShowState(1,p1_state);
    ShowState(2,p2_state);
    ShowState(3,htr_state);
}

void MainScreen::buttonEnter() {
    //enter main menu
    shown = false;
    Jump(menu);
}

void MainScreen::ButtonCallback(uint8_t id, bool hold) {
    if (id == 2 && !hold) buttonEnter();
}

void MainScreen::ShowTimeToNextState(unsigned int index, int val) {
    if (!shown) return;

    ui[index][1].Print(std::to_string(val));
}

void MainScreen::ShowState(unsigned int index, int val) {
    if (!shown) return;

    HeaterState state = static_cast<HeaterState>(val);

    switch(state) {
        case OFF:
            ui[index][1].Clear();
            ui[index][2].Clear();
            ui[index][3].Print("OFF");
            break;
        case DELAY_TO_ON:
            ui[index][2].Print("->");
            ui[index][3].Print("ON");
            break;
        case ON:
            ui[index][1].Clear();
            ui[index][2].Clear();
            ui[index][3].Print("ON");
            break;
        case DELAY_TO_OFF:
            ui[index][2].Print("->");
            ui[index][3].Print("OFF");
            break;
    }
}