#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <Arduino.h>
#include <variant>
#include <string>
#include <map>

class ObjectModel;

enum class ObjectModelGetterReturn {
    OK = 0,
    INVLABEL,
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

class ObjectModel {
    protected:
        ObjectModelItemMap omItems;
        void updateLocalProperty(const std::string &label, const ObjectModelItemValue &value);
    public:
        virtual ObjectModelItemMap &getObjectModel(){ return omItems; };
        std::string serialize();
        ObjectModelGetterReturn getProperty(const std::string &label, ObjectModelItemValue &value);
        ObjectModelSetterReturn setProperty(const std::string &label, const ObjectModelItemValue &value);
};

class ObjectModelList : public ObjectModel {
    public:
        ObjectModelList(const std::vector<ObjectModel*> &children) {
            for (size_t i = 0; i < children.size(); i++) {
                omItems[std::to_string(i)] = ObjectModelItem{children[i]};
            }
        }
        ObjectModel* operator[](size_t index) {
            auto it = omItems.find(std::to_string(index));
            if (it != omItems.end() && std::holds_alternative<ObjectModel*>(it->second.value)) {
                return std::get<ObjectModel*>(it->second.value);
            }
            return nullptr;
        }
};

#endif 