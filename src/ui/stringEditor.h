#ifndef STRING_EDITOR_H
#define STRING_EDITOR_H

#include <Arduino.h>
#include <string>
#include <stdexcept>

#include "ui.h"
#include "navigativeScreen.h"
#include "menuValueIface.h"


class StringEditor
    : public ButtonInteractive
    , public NavigativeScreen
{
    private:
        Ui &ui;
        std::shared_ptr<const MenuValueIface> iface;
        std::string label;
        std::string value;

        std::vector<TextField> keyboardTemplate{};
        std::vector<UiTemplateEntry> editorTemplate{};
        const size_t kbdNumFields = 11;
        const size_t kbdNumMidpoint = 5;
        size_t charPos = 0;

    public:
        StringEditor(Ui &_ui,
                std::shared_ptr<const MenuValueIface> _iface,
                const std::string &_label
        );

        void Show() override;
        void Redraw();
        void ButtonCallback(uint8_t id, bool hold) override;
};

#endif
