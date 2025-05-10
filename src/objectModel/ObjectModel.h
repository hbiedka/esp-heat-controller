#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <Arduino.h>
#include <variant>
#include <string>
#include <map>

class ObjectModel;

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
    // std::string label;
    ObjectModelItemValue value;
    ObjectModelSetter *setter = nullptr; // Pointer to setter function for the item

};

using ObjectModelItemMap = std::map<std::string, ObjectModelItem>;

class ObjectModel {
    protected:
        ObjectModelItemMap omItems;
    public:
        virtual ObjectModelItemMap &getObjectModel(){ return omItems; };
        std::string serialize();
        // int getIndexFromLabel(const std::string &label);
        ObjectModelSetterReturn setProperty(const std::string &label, const ObjectModelItemValue &value);
};

class ObjectModelList : public ObjectModel {
    public:
        ObjectModelList(const std::vector<ObjectModel*> &children) {
            for (size_t i = 0; i < children.size(); i++) {
                omItems[std::to_string(i)] = ObjectModelItem{children[i]};
            }
        }
};

#endif 