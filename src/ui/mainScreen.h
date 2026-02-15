#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "ui.h"
#include "navigativeScreen.h"

static UiBlock block{{
    TextField(12,0,60,0,0)
}};

static const std::vector<UiTemplateEntry> uiTemplate{
    {block,0,0},
};


class MainScreen : public ButtonInteractive, public NavigativeScreen {

    private:
        Ui &ui;
        NavigativeScreen *menu;

    public:
        MainScreen(Ui &_ui, NavigativeScreen *_menu):
            ui(_ui),
            menu(_menu)
        {}

        void Show();
        void buttonEnter();
        void ButtonCallback(uint8_t id, bool hold) override;
};

#endif