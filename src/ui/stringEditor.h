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

        std::vector<TextField> keyboardPtrTemplate{};
        std::vector<TextField> keyboardTemplate{};
        std::vector<UiTemplateEntry> editorTemplate{};
        const size_t kbdNumFields = 9;
        const size_t kbdNumMidpoint = 4;
        size_t charPos = 0;

        enum {
            BACKSPACE,
            CHAR,
            OK
        } cursorPos = BACKSPACE;

        void Redraw();
        void RedrawKeyboard();
    public:
        StringEditor(Ui &_ui,
                std::shared_ptr<const MenuValueIface> _iface,
                const std::string &_label
        );

        void Show() override;
        void ButtonCallback(uint8_t id, bool hold) override;
};

#endif
