#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <string>
#include <memory>
#include "button/buttonGroup.h"
#include "display/oled.h"
#include "display/oledUi.h"

#include "ui.h"
#include "menuValueIface.h"
#include "menuRef.h"

// class Menu;

enum MenuItemType {
    BOOL = 0,   // yes/no
    ENUM,       // list of values
    INT,        // integer
    LINK,       //links to another menu
    BACK        //backs to previous menu
};

struct MenuItem {
    std::string label;                   // Label
    MenuItemType type = BOOL;       // type selector

    const std::vector<std::string> &enumLabels;  // for Bool, alternative labels {"Value if false","Value if true"}
                                            // for Enum, hold enum labels (remember to pass the size in max_value)
                                            // ignored for any other type

    std::shared_ptr<const MenuValueIface> iface;    // interface with get/set methods for int and bool
                                    // and get method for link
    std::string format = "%d";            // format string for int
                                    // ignored for any other type
};

class Menu : ButtonInteractive {
    private:
        std::vector<MenuItem> items;
        ButtonGroup *bg = nullptr;
        Oled *oled = nullptr;

        const unsigned int itemsOnScreen = 4;

        unsigned int pos = 0;
        unsigned int scrollPos = 0;
        bool valueEdit = false;

        Menu *prev = nullptr;

        UiBlock menuItem{{
            TextField(12,0,60,0,0),     //label
            TextField(80,0,40,0,4),     //value
            TextField(2,0,10,1,0),      //pointer
        }};
        Ui ui;

        unsigned int getMenuItemsOnDisplay() { return ui.getBlocksNum(); };

        
        //button callbacks
        void buttonUp();
        void buttonDown();
        void buttonEnter();

        void ButtonCallback(uint8_t id, bool hold) override;

        void Redraw();
        void Redraw(bool forceRedraw);
        void RedrawValueOnPos(unsigned int posToRedraw);

    public:
        Menu(std::vector<MenuItem> _items,ButtonGroup *_bg, Ui &_ui);

        void Show();
        void Show(Menu *_prev);

        void DrawMenuItem();
        void DrawMenuValue();
};

#endif