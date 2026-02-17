#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "ui.h"
#include "navigativeScreen.h"
#include "../objectModel/ObjectModelWatcher.h"
#include "../objectModel/ObjectModel.h"

static UiBlock htrStatus{{
    TextField(2,0,70,0,0),
    TextField(72,0,20,0,0),
    TextField(92,0,10,0,0),
    TextField(102,0,20,0,0),
}};
static UiBlock label{{
    TextField(2,0,120,0,0),
}};

static const std::vector<UiTemplateEntry> uiTemplate{
    {htrStatus,0,6},    //label
    {htrStatus,0,0},    //p1
    {htrStatus,0,2},    //p2
    {htrStatus,0,4},    //htr
};

class HeaterStateWatcher;
class HeaterTimeToNextStateWatcher;

class MainScreen : 
    public ButtonInteractive, 
    public NavigativeScreen
{
    private:
        Ui &ui;
        NavigativeScreen *menu;
        bool shown = false;

        ObjectModel *pump1, *pump2, *htr;

        void ShowTimeToNextState(unsigned int index, int val);
        void ShowState(unsigned int index, int val);

    public:
        friend class HeaterStateWatcher;
        friend class HeaterTimeToNextStateWatcher;

        MainScreen(Ui &_ui, NavigativeScreen *_menu, ObjectModel *pump1,ObjectModel *pump2,ObjectModel *htr);

        void Show();
        void buttonEnter();
        void ButtonCallback(uint8_t id, bool hold) override;
};

class HeaterStateWatcher : public ObjectModelWatcher
{
    private:
        MainScreen *host;
        unsigned int heaterId;
    public:
        HeaterStateWatcher(MainScreen *_host, unsigned int id) :
            host(_host),
            heaterId(id)
        {};
        void operator()(const std::string &property, const ObjectModelItem &item);
};

class HeaterTimeToNextStateWatcher : public ObjectModelWatcher
{
    private:
        MainScreen *host;
        unsigned int heaterId;
    public:
        HeaterTimeToNextStateWatcher(MainScreen *_host, unsigned int id) :
            host(_host),
            heaterId(id)
        {};
        void operator()(const std::string &property, const ObjectModelItem &item);
};

#endif