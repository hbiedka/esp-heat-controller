#ifndef OBJECT_MODEL_TO_MENU_H
#define OBJECT_MODEL_TO_MENU_H
#include <Arduino.h>


#include "objectModel/ObjectModel.h"
#include "ui/menuValueIface.h"

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

class StringObjectModelIface : public MenuValueIface {
    private:
        ObjectModel *objectModel;
        std::string label;
        size_t minLen = 0;
        size_t maxLen = 0;

    public:
        StringObjectModelIface(ObjectModel *om, const std::string &lbl, size_t limit) :
            objectModel(om),
            label(lbl),
            minLen(0),
            maxLen(limit) {}
        StringObjectModelIface(ObjectModel *om, const std::string &lbl, size_t bottom_limit, size_t top_limit) :
            objectModel(om),
            label(lbl),
            minLen(bottom_limit),
            maxLen(top_limit) {}

        MenuGetterReturn get(std::string &ref) const override;
        MenuSetterReturn set(std::string &ref) const override;
};

#endif
