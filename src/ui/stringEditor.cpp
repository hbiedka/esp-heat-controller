#include "stringEditor.h"

std::vector<TextField> editorItem{
    TextField(2,0,60,0,0),     //label
    TextField(2,2,120,0,2,true),     //value
    TextField(2,4,8,0,5),  //backspace pointer
    TextField(2,6,8,0,0),  //backspace
    TextField(110,4,18,0,5),  //ok pointer
    TextField(110,6,18,0,0),  //ok
};

static const std::string brick{ (char)127 };
static const std::string backsp{ (char)129 };

const std::string chars{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnoprqrstuvwxyz1234567890!@#$%^&*,./?-_=+;:'\"[]{}<>\\|"};

StringEditor::StringEditor(Ui &_ui,
        std::shared_ptr<const MenuValueIface> _iface,
        const std::string &_label
    )
    : ui(_ui)
    , iface(_iface)
    , label(_label)
{

    for (size_t i = 0; i < kbdNumFields; i++) {
        keyboardPtrTemplate.push_back(TextField(i*10,0,10,0,5));
        keyboardTemplate.push_back(TextField(i*10,0,10,0,0));
    }

    editorTemplate = std::vector<UiTemplateEntry>{
        {editorItem,0,0},
        {keyboardPtrTemplate,12,4},
        {keyboardTemplate,12,6}
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
    ui[0][3].Print(backsp);
    ui[0][5].Print("OK");

    Redraw();
}

// Redraws the value and the keyboard with the current cursor position and character position
void StringEditor::Redraw() {

    ui[0][1].Print(value);
    RedrawKeyboard();
}

// Redraws the keyboard with the current cursor position and character position
void StringEditor::RedrawKeyboard() {

    //calculate offset between all and shown chars on kbd
    if (charPos >= chars.size()) charPos = chars.size()-1;
    size_t displayOffset =
        charPos < kbdNumMidpoint ? 0 : charPos- kbdNumMidpoint;

    if (displayOffset > chars.size() - kbdNumFields)
        displayOffset = chars.size() - kbdNumFields;

    for (size_t i = 0; i < kbdNumFields; i++) {
        size_t posToShow = displayOffset+i;
        if (posToShow < chars.size()) {
            if (cursorPos == CHAR && posToShow == charPos)
                ui[1][i].Print(brick);
            else
                ui[1][i].Clear();

            ui[2][i].Print(std::string{chars.at(posToShow)});
        }
    }

    if (cursorPos == BACKSPACE)
        ui[0][2].Print(brick);
    else
        ui[0][2].Clear();

    if (cursorPos == OK)
        ui[0][4].Print(brick);
    else
        ui[0][4].Clear();

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
            if ( cursorPos == OK) {
                JumpBack();
            } else {
                if (cursorPos == CHAR)
                    value += chars.at(charPos);
                else if (cursorPos == BACKSPACE && value.size() > 0)
                    value.pop_back();
                Redraw();
            }
        }
    }
    else if (id == 1) {
        if (cursorPos == CHAR) {
            if (charPos > 0)
                charPos--;
            else
                cursorPos = BACKSPACE;
            RedrawKeyboard();
        } else if (cursorPos == OK) {
            cursorPos = CHAR;
            charPos = chars.size()-1;
            RedrawKeyboard();
        }
    }
    else if (id == 3) {

        if (cursorPos == CHAR) {
            if (charPos < chars.size()-1)
                charPos++;
            else
                cursorPos = OK;
            RedrawKeyboard();
        } else if (cursorPos == BACKSPACE) {
            cursorPos = CHAR;
            charPos = 0;
            Redraw();
        }
    }
}
