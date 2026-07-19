#include "mainScreen.h"
#include "../heat/heater.h"

void MainScreenWatcher::operator()(
            const std::string &property,
            const std::string &context,
            const ObjectModelItem &item)
{
    if (!host) return;  //TODO throw exception
    host->handleObjectModelChange(property, context, item);
}

MainScreen::MainScreen(Ui &_ui, NavigativeScreen *_menu,ObjectModel &_om):
    ui(_ui),
    menu(_menu),
    om(_om),
    watcher(this)
{
    om["pumps"]["0"].AddWatcher("state","pumps/0/state",&watcher);
    om["pumps"]["1"].AddWatcher("state","pumps/1/state",&watcher);
    om["heater"].AddWatcher("state","heater/state",&watcher);
    om["pumps"]["0"].AddWatcher("timeToNextState","pumps/0/timeToNextState",&watcher);
    om["pumps"]["1"].AddWatcher("timeToNextState","pumps/1/timeToNextState",&watcher);
    om["heater"].AddWatcher("timeToNextState","heater/timeToNextState",&watcher);
    om["wifi"].AddWatcher("state","wifi/state",&watcher);
}

MainScreen::~MainScreen()
{
    om["pumps"]["0"].RemoveWatcher("state",watcher);
    om["pumps"]["1"].RemoveWatcher("state",watcher);
    om["heater"].RemoveWatcher("state",watcher);
    om["pumps"]["0"].RemoveWatcher("timeToNextState",watcher);
    om["pumps"]["1"].RemoveWatcher("timeToNextState",watcher);
    om["heater"].RemoveWatcher("timeToNextState",watcher);
    om["wifi"].RemoveWatcher("state",watcher);
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
    om["pumps"][0].getInt("state",p1_state);
    om["pumps"][1].getInt("state",p2_state);
    om["heater"].getInt("state",htr_state);

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
    //wake up display first
    if (ui.OledWakeUpPulse()) return;

    if (id == 2 && !hold) buttonEnter();
}

void MainScreen::handleObjectModelChange(const std::string &property, const std::string &context, const ObjectModelItem &item)
{
    if (!shown) return;
    //check property type
    if (!std::holds_alternative<int>(item.value)) return; //TODO throw exception

    if (context == "pumps/0/state") {
        ShowState(1,std::get<int>(item.value));
    }
    else if (context == "pumps/1/state") {
        ShowState(2,std::get<int>(item.value));
    }
    else if (context == "heater/state") {
        ShowState(3,std::get<int>(item.value));
    }
    else if (context == "pumps/0/timeToNextState") {
        ShowTimeToNextState(1,std::get<int>(item.value));
    }
    else if (context == "pumps/1/timeToNextState") {
        ShowTimeToNextState(2,std::get<int>(item.value));
    }
    else if (context == "heater/timeToNextState") {
        ShowTimeToNextState(3,std::get<int>(item.value));
    }
    else if (context == "wifi/state") {
        int state = std::get<int>(item.value);
        ui[0][0].Clear();
        switch(state) {
            case 0:
                ui[0][0].Print("WiFi: disconnected");
                break;
            case 1:
            case 2:
                ui[0][0].Print("WiFi: connecting");
                break;
            case 3:
                ui[0][0].Print("WiFi: connected");
                break;
        }
    }
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