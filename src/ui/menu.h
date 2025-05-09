#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <string>
#include "button/buttonGroup.h"
#include "display/oled.h"
#include "display/oledUi.h"

#include "menuValueIface.h"

class Menu;

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

    const MenuValueIface &iface;    // interface with get/set methods for int and bool
                                    // and get method for link
};

class Menu {
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
            TextField(20,0,60,0,0),     //label
            TextField(80,0,40,0,4),     //value
            TextField(2,0,15,1,0),      //pointer

        }};
        Ui ui{{
            {menuItem,0,0},
            {menuItem,0,2},
            {menuItem,0,4},
            {menuItem,0,6}
        }};

        unsigned int getMenuItemsOnDisplay() { return ui.getBlocksNum(); };

        
        //button callbacks
        void buttonUp();
        void buttonDown();
        void buttonEnter();

        void buttonCb(uint8_t id);
        void buttonHoldCb(uint8_t id);

        void Redraw();
        void Redraw(bool forceRedraw);
        void RedrawValueOnPos(unsigned int posToRedraw);

    public:
        friend void buttonCbWrapper(uint8_t id);
        friend void buttonHoldCbWrapper(uint8_t id);

        Menu(std::vector<MenuItem> _items,ButtonGroup *_bg, Oled *_oled);

        void Show();
        void Show(Menu *_prev);

        void DrawMenuItem();
        void DrawMenuValue();
};

#endif