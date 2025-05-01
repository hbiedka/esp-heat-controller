#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <Arduino.h>
#include <variant>
#include <string>
#include <vector>

class ObjectModel;

enum class ObjectModelItemType {
    BOOL = 0,
    INT,
    STRING,
    LINK
};

using ObjectModelItemValue = std::variant<int, bool, std::string>;
using ObjectModelSetter = int (*)(const ObjectModelItemValue&);

struct ObjectModelItem {
    std::string label;
    ObjectModelItemType type;
    ObjectModelItemValue value;
    ObjectModel *link = nullptr; // Pointer to another ObjectModel if type is LINK
    ObjectModelSetter setter = nullptr; // Setter function for the item

};

using ObjectModelItemList = std::vector<ObjectModelItem>;

class ObjectModel {
    protected:
        ObjectModelItemList omItems;
    public:
        virtual ObjectModelItemList &getObjectModel(){ return omItems; };
        std::string serialize();
};

#endif 