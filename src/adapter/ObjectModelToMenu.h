#ifndef OBJECT_MODEL_TO_MENU_H
#define OBJECT_MODEL_TO_MENU_H
#include <Arduino.h>


#include "objectModel/ObjectModel.h"
#include "ui/menuValueIface.h"
#include "ui/menu.h"

class BoolObjectModelIface : public MenuValueIface {
    private:
        ObjectModel *objectModel;
        std::string label;
    
    public:
        BoolObjectModelIface(ObjectModel *om, const std::string &lbl) : objectModel(om), label(lbl) {}
        MenuGetterReturn get(bool &ref) const override;
        MenuSetterReturn set(bool &ref) const override;
};

class IntObjectModelIface : public MenuValueIface {
    private:
        ObjectModel *objectModel;
        std::string label;
        int floor = 0;
        int ceil = 0;
    
    public:
        IntObjectModelIface(ObjectModel *om, const std::string &lbl, int limit) : 
            objectModel(om), 
            label(lbl),
            floor(0),
            ceil(limit) {}
        IntObjectModelIface(ObjectModel *om, const std::string &lbl, int bottom_limit, int top_limit) : 
            objectModel(om), 
            label(lbl),
            floor(bottom_limit),
            ceil(top_limit) {}
        
        IntObjectModelIface(ObjectModel *om, const std::string &lbl) : objectModel(om), label(lbl) {}
        MenuGetterReturn get(int &ref) const override;
        MenuSetterReturn set(int &ref) const override;
};

#endif
