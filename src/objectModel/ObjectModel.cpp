#include "ObjectModel.h"

std::string serializeValue(const ObjectModelItemValue &value) {
    if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value) ? "true" : "false";
    }
    if (std::holds_alternative<int>(value)) {
        return std::to_string(std::get<int>(value));
    } 
    if (std::holds_alternative<std::string>(value)) {
        // Assuming std::string is a class with a method to get the C-string representation
        return "\"" + std::get<std::string>(value) + "\""; // Uncomment if std::string has a method to get C-string
    }

    return "<unknown>";

}


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
        if (std::holds_alternative<ObjectModel*>(item.value)) {
            ObjectModel *linked_ob = std::get<ObjectModel*>(item.value);
            if (linked_ob != nullptr)
                result += linked_ob->serialize();
            else
                result += "{}";
        } else {
            result += serializeValue(item.value);
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

ObjectModelGetterReturn ObjectModel::getBool(const std::string &label, bool &value)
{
    ObjectModelItemValue propValue;
    ObjectModelGetterReturn ret;
    
    ret = getProperty(label,propValue);
    if (ret != ObjectModelGetterReturn::OK)
        return ret;

    if (!std::holds_alternative<bool>(propValue))
        return ObjectModelGetterReturn::INVTYPE;
    
    value = std::get<bool>(propValue);
    return ObjectModelGetterReturn::OK;
}

ObjectModelGetterReturn ObjectModel::getInt(const std::string &label, int &value)
{
    ObjectModelItemValue propValue;
    ObjectModelGetterReturn ret;
    
    ret = getProperty(label,propValue);
    if (ret != ObjectModelGetterReturn::OK)
        return ret;

    if (!std::holds_alternative<int>(propValue))
        return ObjectModelGetterReturn::INVTYPE;
    
    value = std::get<int>(propValue);
    return ObjectModelGetterReturn::OK;
}

ObjectModelGetterReturn ObjectModel::getString(const std::string &label, std::string &value)
{
    ObjectModelItemValue propValue;
    ObjectModelGetterReturn ret;
    
    ret = getProperty(label,propValue);
    if (ret != ObjectModelGetterReturn::OK)
        return ret;

    if (!std::holds_alternative<std::string>(propValue))
        return ObjectModelGetterReturn::INVTYPE;
    
    value = std::get<std::string>(propValue);
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

void ObjectModel::updateLocalProperty(const std::string &label, const ObjectModelItemValue &value)
{
    //find index
    auto it = omItems.find(label);
    if (it == omItems.end()) {
        //no label like this
        //TODO throw exception
        return;
    }

    if (it->second.value != value) {
        //update value
        it->second.value = value;

        //look for watchers
        for (auto watcher : watchers) {
            if (watcher.property == it->first && watcher.object) {
                watcher.object->WatchCallback(watcher.id,it->second);
            }
        }
    }
}