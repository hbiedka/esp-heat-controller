#include "menu.h"

String defaultBoolLabels[2] = {"Y","N"};

Menu *currentButtonCbHolder = nullptr;

void buttonCbWrapper(uint8_t id) {
    if (currentButtonCbHolder != nullptr) currentButtonCbHolder->buttonCb(id);
}
void buttonHoldCbWrapper(uint8_t id){
    if (currentButtonCbHolder != nullptr) currentButtonCbHolder->buttonHoldCb(id);
}

Menu::Menu(std::vector<MenuItem> _items,ButtonGroup *_bg, Oled *_oled): 
    items(_items),
    bg(_bg),
    oled(_oled)
{
    ui.SetOled(oled);
}

void Menu::Show(Menu *_prev) {
    //save previous menu
    prev = _prev;
    Show();
}

//up/down buttons callbacks
void Menu::buttonUp() {
    if(pos > 0) pos--;
    Redraw();

}
void Menu::buttonDown() {
    if(pos < items.size()-1 ) pos++;
    Redraw();

}

//callback when the Enter key is pressed
void Menu::buttonEnter() {
    MenuItem &item = items[pos];
    
    //if bool
    if (item.type == BOOL) {
        //change state
        bool *val = item.value.boolean;
        *val = !*val;

    } else if (item.type == ENUM) {
        int *val = item.value.numeric;
        (*val)++;
        if (*val >= item.maxVal) *val = 0;
    }
    
    //if intenger
        //enter the integer edit
    //if link
        //set previous 
        //go to link
    //if back
        //if previous is not null
            //go to previous
    RedrawValueOnPos(pos);

}

void Menu::buttonCb(uint8_t id) {
    // Serial.printf("buttonCb id %u\n",id);
    switch(id) {
        case 1: buttonUp(); break;
        case 2: buttonEnter(); break;
        case 3: buttonDown(); break;
        default: break;
    }
}

void Menu::buttonHoldCb(uint8_t id) {
    //TODO
}


// enter the menu,  refresh display, set button callbacks
void Menu::Show() {
    if (bg != nullptr) {
        //set button callbacks
        currentButtonCbHolder = this;
        bg->setCallbackForAll(buttonCbWrapper);
        bg->setHoldCallbackForAll(buttonHoldCbWrapper);
    } 
    
    //draw all
    Redraw(true);
}

//called every time when some action occured eg. button pressed
// forceRedraw is true when eg. draws after moving from another menu
void Menu::Redraw(boolean forceRedraw) {
    //control scroll position

    bool scrolled = false;
    bool cleared = false;
    unsigned int i;

    if (pos < scrollPos) {
        scrollPos = pos;
        scrolled = true;
    } else if (pos >= scrollPos + getMenuItemsOnDisplay()) {
        scrollPos = pos - getMenuItemsOnDisplay() + 1;
        scrolled = true;
    }

    //if scroled or redraw forced
    if (scrolled || forceRedraw) {
        //redraw all
        oled->Clear();
        cleared = true;

        unsigned int posWScrollOffset; 
        for (i = 0; i < getMenuItemsOnDisplay(); i++) {
            posWScrollOffset = i + scrollPos;
            if ( posWScrollOffset < items.size() ) {

                ui[i][0].Print(items[posWScrollOffset].label);
                RedrawValueOnPos(posWScrollOffset);
            }
        }
    }
    //move pointer
    if (!cleared) {
        for (i = 0; i < getMenuItemsOnDisplay(); i++) {
            ui[i][2].Clear();
        }
    }
    
    unsigned int ptrPos = pos-scrollPos;
    if (ptrPos < getMenuItemsOnDisplay() ){
        ui[ptrPos][2].Print(">");
    }
    //Else throw exception?

}

void Menu::RedrawValueOnPos(unsigned int posToRedraw) {
    //check if the value is currently shown
    if (posToRedraw < scrollPos) return;
    if (posToRedraw > (scrollPos + getMenuItemsOnDisplay() -1) ) return;

    unsigned int uiPosToRedraw = posToRedraw-scrollPos;

    MenuItem &item = items[posToRedraw];
    
    //TODO this one will be separate function to handle every possible case
    if (item.type == BOOL && item.value.boolean != nullptr ) {
        String *labels = item.enumLabels != nullptr ? item.enumLabels : defaultBoolLabels;

        bool *val = item.value.boolean;
        ui[uiPosToRedraw][1].Print( labels[*val ? 1 : 0] );
    } else if (item.type == ENUM && item.value.numeric != nullptr && item.enumLabels != nullptr ) {
        //enumLabels ptr is mandatory
        int *val = item.value.numeric;
        
        //deal with negative values
        //TODO throw exception
        if (*val < 0) *val = 0;

        ui[uiPosToRedraw][1].Print( item.enumLabels[*val] );
    }

}

void Menu::Redraw() {
    Redraw(false);
}

//draws menu item
void Menu::DrawMenuItem() {
    //draw label
    //drawMenuValue
}

//draw menu value
void Menu::DrawMenuValue() {
}