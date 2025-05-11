#include "ObjectModel.h"

std::string ObjectModel::serialize() {
    ObjectModelItemMap &om = getObjectModel();

    std::string result = "{";
    bool first_run = true;
    
    for ( auto& map_item : om ) {
        const auto& label = map_item.first;
        const auto& item = map_item.second;
        
        // Add comma if not first item
        if (first_run) {
            first_run = false;
        } else {
            result += ",";
        }
        
        // Add property name
        result += "\"" + label + "\":";
        
        // Add value based on type
        if (std::holds_alternative<bool>(item.value)) {
            result += std::get<bool>(item.value) ? "true" : "false";
        } else if (std::holds_alternative<int>(item.value)) {
            result += std::to_string(std::get<int>(item.value));
        } else if (std::holds_alternative<std::string>(item.value)) {
            // Assuming std::string is a class with a method to get the C-string representation
            result += "\"" + std::get<std::string>(item.value) + "\""; // Uncomment if std::string has a method to get C-string
        } else if (std::holds_alternative<ObjectModel*>(item.value)) {
            ObjectModel *linked_ob = std::get<ObjectModel*>(item.value);
            if (linked_ob != nullptr)
                result += linked_ob->serialize();
            else
                result += "{}";
        } else {
            result += "null";
        }
    }
    
    result += "}";
    return result;
}

ObjectModelGetterReturn ObjectModel::getProperty(const std::string &label, ObjectModelItemValue &value) {

    ObjectModelItemMap &om = getObjectModel();
    //find index
    auto it = om.find(label);
    if (it == om.end())
        //no label like this
        return ObjectModelGetterReturn::INVLABEL;

    value = it->second.value;
    return ObjectModelGetterReturn::OK;
}

ObjectModelSetterReturn ObjectModel::setProperty(const std::string &label, const ObjectModelItemValue &value) {

    //find index
    auto it = omItems.find(label);
    if (it == omItems.end())
        //no label like this
        return ObjectModelSetterReturn::INVLABEL;

    if (it->second.setter == nullptr)
        return ObjectModelSetterReturn::READONLY;

    return (*it->second.setter)(label,value);
}