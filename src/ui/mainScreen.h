#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "ui.h"
#include "navigativeScreen.h"
#include "../objectModel/ObjectModelWatcher.h"
#include "../objectModel/ObjectModel.h"

static std::vector<TextField> htrStatus{{
    TextField(2,0,70,0,0),
    TextField(72,0,20,0,0),
    TextField(92,0,10,0,0),
    TextField(102,0,20,0,0),
}};
static std::vector<TextField> label{{
    TextField(2,0,120,0,0),
}};

static const std::vector<UiTemplateEntry> uiTemplate{
    {label,0,6},    //label
    {htrStatus,0,0},    //p1
    {htrStatus,0,2},    //p2
    {htrStatus,0,4},    //htr
};

class HeaterStateWatcher;
class HeaterTimeToNextStateWatcher;

class MainScreen;

class MainScreenWatcher : public ObjectModelWatcher
{
    private:
        MainScreen *host;
    public:
        MainScreenWatcher(MainScreen *_host) :
            host(_host)
        {};
        void operator()(
            const std::string &property,
            const std::string &context,
            const ObjectModelItem &item) override;
};

class MainScreen :
    public ButtonInteractive,
    public NavigativeScreen
{
    private:
        Ui &ui;
        NavigativeScreen *menu;
        bool shown = false;

        ObjectModel &om;
        MainScreenWatcher watcher;

        void handleObjectModelChange(const std::string &property, const std::string &context, const ObjectModelItem &item);
        void ShowTimeToNextState(unsigned int index, int val);
        void ShowState(unsigned int index, int val);

    public:
        friend class MainScreenWatcher;

        MainScreen(Ui &_ui, NavigativeScreen *_menu, ObjectModel &_om);
        ~MainScreen();

        void Show();
        void buttonEnter();
        void ButtonCallback(uint8_t id, bool hold) override;
};

#endif