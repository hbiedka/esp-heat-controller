#ifndef OBJECT_MODEL_COMMON_H
#define OBJECT_MODEL_COMMON_H

#include <Arduino.h>
#include <map>
#include <string>
#include <variant>

class ObjectModel;
class ObjectModelWatcher;

enum class ObjectModelGetterReturn {
    OK = 0,
    INVLABEL,
    INVTYPE,
    NOT_IMPLEMENTED
};

enum class ObjectModelSetterReturn {
    OK = 0,
    INVTYPE,
    INVVAL,
    INVLABEL,
    INVFUNC,
    READONLY,
    NOT_IMPLEMENTED
};

using ObjectModelItemValue = std::variant<int, bool, std::string, ObjectModel* >;

class ObjectModelSetter {
    public:
        virtual ObjectModelSetterReturn operator()(const std::string &label, const ObjectModelItemValue &value) {
            return ObjectModelSetterReturn::NOT_IMPLEMENTED;
        }
        virtual ~ObjectModelSetter() = default;
};

struct ObjectModelItem {
    ObjectModelItemValue value;
    ObjectModelSetter *setter = nullptr; // Pointer to setter function for the item
};

using ObjectModelItemMap = std::map<std::string, ObjectModelItem>;

std::string serializeValue(const ObjectModelItemValue &value);

#endif