#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <string>
#include <memory>

#include "ui.h"
#include "navigativeScreen.h"
#include "menuValueIface.h"
#include "menuRef.h"

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
    Menu *link = nullptr;                           // Link to menu item

    std::string format = "%d";            // format string for int
                                    // ignored for any other type

    MenuItem(const std::string &label, MenuItemType type,
             const std::vector<std::string> &enumLabels,
             std::shared_ptr<const MenuValueIface> iface,
             const std::string &format)
        : label(label), type(type), enumLabels(enumLabels), iface(std::move(iface)), format(format) {}

    MenuItem(const std::string &label, MenuItemType type,
             const std::vector<std::string> &enumLabels,
             std::shared_ptr<const MenuValueIface> iface)
        : label(label), type(type), enumLabels(enumLabels), iface(std::move(iface)) {}

    MenuItem(const std::string &label, MenuItemType type,
             std::shared_ptr<const MenuValueIface> iface,
             const std::string &format)
        : label(label), type(type), enumLabels(std::vector<std::string>()), iface(std::move(iface)), format(format) {}

    MenuItem(const std::string &label, MenuItemType type,
             std::shared_ptr<const MenuValueIface> iface)
        : label(label), type(type), enumLabels(std::vector<std::string>()), iface(std::move(iface)) {}

    MenuItem(const std::string &label, MenuItemType type,
            Menu *_link
        )
        : label(label), type(type), enumLabels(std::vector<std::string>())
        {
            //only LINK can Pass a menu element
            if (type == LINK) {
                link = _link;
                iface = std::make_shared<MenuValueIface>();
            }
            //else TODO throw exception
        }

    MenuItem(const std::string &label, MenuItemType type)
        : label(label), type(type), enumLabels(std::vector<std::string>())
        {
            //only BACK can skip an iface
            if (type == BACK) {
                iface = std::make_shared<MenuValueIface>();
            }
            //else TODO throw exception
        }
};

class Menu : public ButtonInteractive, public NavigativeScreen {
    private:
        Ui &ui;
        std::vector<MenuItem> items;

        const unsigned int itemsOnScreen = 4;

        unsigned int pos = 0;
        unsigned int scrollPos = 0;
        bool valueEdit = false;

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
        Menu(Ui &_ui, std::vector<MenuItem> _items);

        void Show() override;

        void DrawMenuItem();
        void DrawMenuValue();
};

#endif