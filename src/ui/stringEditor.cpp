#include "stringEditor.h"

std::vector<TextField> editorItem{
    TextField(2,0,60,0,0),     //label
    TextField(2,2,120,0,0),     //value
};

const std::string chars{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnoprqrstuvwxyz1234567890!@#$%^&*,./?-_=+;:'\"[]{}<>\\|"};

StringEditor::StringEditor(Ui &_ui,
        std::shared_ptr<const MenuValueIface> _iface,
        const std::string &_label
    )
    : ui(_ui)
    , iface(_iface)
    , label(_label)
{

    //TODO keyboard
    for (size_t i = 0; i < kbdNumFields; i++) {
        keyboardTemplate.push_back(TextField(2+i*10,0,10,0,0));
    }

    editorTemplate = std::vector<UiTemplateEntry>{
        {editorItem,0,0},
        {keyboardTemplate,0,4},
        {keyboardTemplate,0,6}
    };

}

void StringEditor::Show()
{
    ui.takeoverButtons(this);
    ui.loadTemplate(editorTemplate);
    ui.Clear();

    auto ret = iface->get(value);
    if (ret != MenuGetterReturn::OK)
        throw std::runtime_error("Getter returned invalid return code");

    ui[0][0].Print(label);

    Redraw();
}

void StringEditor::Redraw() {

    ui[0][1].Print(value);

    //calculate offset between all and shown chars on kbd
    if (charPos >= chars.size()) charPos = chars.size()-1;
    size_t displayOffset =
        charPos < kbdNumMidpoint ? 0 : charPos- kbdNumMidpoint;

    if (displayOffset > chars.size() - kbdNumFields)
        displayOffset = chars.size() - kbdNumFields;

    for (size_t i = 0; i < kbdNumFields; i++) {
        size_t posToShow = displayOffset+i;
        if (posToShow < chars.size()) {
            if (posToShow == charPos)
                ui[1][i].Print("|");
            else
                ui[1][i].Clear();

            ui[2][i].Print(std::string{chars.at(posToShow)});
        }
    }

}

void StringEditor::ButtonCallback(uint8_t id, bool hold)
{
    //wake up display first
    if (ui.OledWakeUpPulse()) return;

    if (id == 2 ) {
        if (hold) {
            //TODO set
            JumpBack();
        } else {
            value += chars.at(charPos);
            Redraw();
        }
    }
    else if (id == 1) {
        if (charPos > 0) {
            charPos--;
            Redraw();
        }
    }
    else if (id == 3) {
        if (charPos < chars.size()-1) {
            charPos++;
            Redraw();
        }
    }
}
