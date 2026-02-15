#include "menu.h"

std::vector<std::string> defaultBoolLabels{"Y","N"};

Menu *currentButtonCbHolder = nullptr;

void buttonCbWrapper(uint8_t id) {
    if (currentButtonCbHolder != nullptr) currentButtonCbHolder->buttonCb(id);
}
void buttonHoldCbWrapper(uint8_t id){
    if (currentButtonCbHolder != nullptr) currentButtonCbHolder->buttonHoldCb(id);
}

Menu::Menu(std::vector<MenuItem> _items,ButtonGroup *_bg, Ui &_ui): 
    items(_items),
    bg(_bg),
    ui(_ui)
{
    ui.loadTemplate({
        {menuItem,0,0},
        {menuItem,0,2},
        {menuItem,0,4},
        {menuItem,0,6}
    });
}

void Menu::Show(Menu *_prev) {
    //save previous menu
    prev = _prev;
    Show();
}

//up/down buttons callbacks
void Menu::buttonUp() {
    if (valueEdit) {
        MenuItem &item = items[pos];
        MenuSetterReturn ret;

        if (item.type == ENUM) {
            int val;
            if (item.iface->get(val) == MenuGetterReturn::OK) {
                ret = item.iface->set(++val);
                if (ret == MenuSetterReturn::VAL_TOO_BIG) {
                    val = 0;
                    ret = item.iface->set(val); 
                }
            }

        } else if (item.type == INT) {
            int val;
            if (item.iface->get(val) == MenuGetterReturn::OK)
                ret = item.iface->set(++val);
        }
        RedrawValueOnPos(pos);
    } else {
        if(pos > 0) pos--;
        Redraw();
    }

}
void Menu::buttonDown() {
    if (valueEdit) {
        MenuItem &item = items[pos];
        MenuSetterReturn ret;

        if (item.type == ENUM) {
            int val;
            if (item.iface->get(val) == MenuGetterReturn::OK) {
                ret = item.iface->set(--val);
                if (ret == MenuSetterReturn::VAL_TOO_SMALL) {
                    // we don't know the max limit, but we can bump the value until it reach
                    // the ceiling
                    val = 0;
                    do {
                        ret = item.iface->set(++val);
                    } while (ret != MenuSetterReturn::VAL_TOO_BIG);
                }
            }

        } else if (item.type == INT) {
            int val;
            if (item.iface->get(val) == MenuGetterReturn::OK)
                ret = item.iface->set(--val);

        }
        RedrawValueOnPos(pos);
    } else {
        if(pos < items.size()-1 ) pos++;
        Redraw();
    }
}

//callback when the Enter key is pressed
void Menu::buttonEnter() {
    MenuItem &item = items[pos];
    MenuSetterReturn ret;
    
    //if bool
    if (item.type == BOOL) {
        //change state
        bool val;
        if (item.iface->get(val) == MenuGetterReturn::OK) {
            val = !val;
            ret = item.iface->set(val);
            if (ret != MenuSetterReturn::OK) {
                //TODO throw exception
                Serial.printf("ERROR setting %s: %u\n",item.label.c_str(),static_cast<unsigned int>(ret));
            }
        }

    } else if (item.type == ENUM || item.type == INT) {
        valueEdit = !valueEdit;
        Redraw(true);
    } else if (item.type == LINK) {
        Menu *linkMenu = nullptr;
        if (item.iface->get(linkMenu) == MenuGetterReturn::OK) {
            if (linkMenu == nullptr) {
                return;
            }
            linkMenu->Show(this);
            return;
        }
    } else if (item.type == BACK) {
        //go to previous menu
        if (prev != nullptr) {
            prev->Show();
            return;
        }
    }
    
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
    //enter button does not provide on-hold action
    switch(id) {
        case 1: buttonUp(); break;
        case 3: buttonDown(); break;
        default: break;
    }
}


// enter the menu,  refresh display, set button callbacks
void Menu::Show() {
    if (bg != nullptr) {
        //set button callbacks
        currentButtonCbHolder = this;
        bg->setCallbackForAll(buttonCbWrapper);
        bg->setHoldCallbackForAll(buttonHoldCbWrapper);
    } 
    valueEdit = false;
    
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
        ui.Clear();
        cleared = true;

        unsigned int posWScrollOffset; 
        for (i = 0; i < getMenuItemsOnDisplay(); i++) {
            posWScrollOffset = i + scrollPos;
            //if in value edit mode, draw only selected item
            if (valueEdit && posWScrollOffset != pos) continue;

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
        ui[ptrPos][2].Print( items[pos].type == BACK ? "<" : ">" );
    }
    //Else throw exception?

}

void Menu::RedrawValueOnPos(unsigned int posToRedraw) {
    //check if the value is currently shown
    if (posToRedraw < scrollPos) return;
    if (posToRedraw > (scrollPos + getMenuItemsOnDisplay() -1) ) return;

    unsigned int uiPosToRedraw = posToRedraw-scrollPos;

    MenuItem &item = items[posToRedraw];
    MenuGetterReturn ret;
    
    if (item.type == BOOL ) {
        std::vector<std::string> &labels = defaultBoolLabels;
        if (item.enumLabels.size() >= 2)
            labels = item.enumLabels;

        bool val;
        ret = item.iface->get(val);

        if (ret == MenuGetterReturn::OK)
            ui[uiPosToRedraw][1].Print( labels[val ? 1 : 0] );
    } else if (item.type == ENUM && item.enumLabels.size() > 0 ) {
        int val;
        ret = item.iface->get(val);
        
        //deal with negative values and enum overflow
        //TODO throw exception
        if (val < 0) val = 0;
        if (static_cast<unsigned int>(val) >= item.enumLabels.size())
            val = item.enumLabels.size()-1;

        if (ret == MenuGetterReturn::OK)
            ui[uiPosToRedraw][1].Print( item.enumLabels[val] );
    } else if (item.type == INT ) {
        int val;
        ret = item.iface->get(val);

        if (ret == MenuGetterReturn::OK) {
            char buffer[10];
            snprintf(buffer, sizeof(buffer), item.format.c_str(), val);
            ui[uiPosToRedraw][1].Print( std::string(buffer) );
        }
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