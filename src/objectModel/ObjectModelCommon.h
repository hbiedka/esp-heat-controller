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
    TOO_MANY,
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
    bool saveToNVM = false;              // Will be saved and restored from NVM
    size_t NVMStringLen = 0;             // how many bytes to reserve in NVM (for strings only)

    ObjectModelItem() = default;
    ObjectModelItem(ObjectModelItemValue val) : 
        value(val) {}
    ObjectModelItem(ObjectModelItemValue val, ObjectModelSetter *set) : 
        value(val), setter(set) {}
    ObjectModelItem(ObjectModelItemValue val, ObjectModelSetter *set, bool save) : 
        value(val), setter(set), saveToNVM(save) {}
    ObjectModelItem(ObjectModelItemValue val, ObjectModelSetter *set, bool save, size_t nvmStrLen) : 
        value(val), setter(set), saveToNVM(save), NVMStringLen(nvmStrLen) {}

    // Factory methods for creating ObjectModelItems
    static ObjectModelItem createStringItem(const std::string &str, ObjectModelSetter *setter) {
        return ObjectModelItem{str, setter, false};
    }
    static ObjectModelItem createStringItem(const std::string &str, ObjectModelSetter *setter, size_t nvmStrLen) {
        return ObjectModelItem{str, setter, true, nvmStrLen};
    }

    static ObjectModelItem createIntItem(int value, ObjectModelSetter *setter, bool saveToNVM = false) {
        return ObjectModelItem{value, setter, saveToNVM};
    }

    static ObjectModelItem createBoolItem(bool value, ObjectModelSetter *setter, bool saveToNVM = false) {
        return ObjectModelItem{value, setter, saveToNVM};
    }

    };

using ObjectModelItemMap = std::map<std::string, ObjectModelItem>;

std::string serializeValue(const ObjectModelItemValue &value);

#endif