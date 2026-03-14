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

//load from NVM payload
std::vector<unsigned char>::iterator ObjectModel::NVMLoad(std::vector<unsigned char>::iterator it, std::vector<unsigned char>::iterator end)
{
    auto ptr = it;
    ObjectModelItemMap &om = getObjectModel();
    ObjectModelItemValue newValue{false};

    for ( auto& map_item : om ) {
        auto label = map_item.first;
        auto item = map_item.second;
        auto value = item.value;

        if (std::holds_alternative<ObjectModel*>(value)) {
            ObjectModel *linked_ob = std::get<ObjectModel*>(value);
            ptr = linked_ob->NVMLoad(ptr, end);
            if (ptr == end) break;
            continue;
        }

        if (!item.saveToNVM)
            continue;

        if (std::holds_alternative<bool>(value)) {
            // *ptr = std::get<bool>(value) ? 0 : 1;
            bool newBool = !!(*ptr);
            ptr++;
            if (ptr == end) break;

            newValue = ObjectModelItemValue{newBool};

        }
        else if (std::holds_alternative<int>(value)) {
            // convert from char array
            int newInt;
            size_t size = sizeof(int);

            char bytes[size];
            for (size_t i = 0; i < size; i++) {
                bytes[i] = *ptr;
                ptr++;
                if (ptr == end) break;

            }

            memcpy(&newInt,bytes,size);
            newValue = ObjectModelItemValue{newInt};
        }
        else if (std::holds_alternative<std::string>(value)) {
            std::string newStr;
            for (size_t i = 0; i < item.NVMStringLen; i++) {
                char c = *ptr;
                ptr++;
                if (ptr == end) break;

                if (c == 0) break; // stop at null terminator
                newStr += c;
            }
            newValue = ObjectModelItemValue{newStr};
        }

        auto ret = setProperty(label,newValue);
        if (ret != ObjectModelSetterReturn::OK) {
            //TODO throw exception
            break;
        }
    }
    return ptr;
}

//store in NVM payload
std::vector<unsigned char>::iterator ObjectModel::NVMDump(std::vector<unsigned char>::iterator it, std::vector<unsigned char>::iterator end)
{
    auto ptr = it;

    ObjectModelItemMap &om = getObjectModel();

    for ( auto& map_item : om ) {
        auto item = map_item.second; 
        auto value = item.value;

        if (std::holds_alternative<ObjectModel*>(value)) {
            ObjectModel *linked_ob = std::get<ObjectModel*>(value);
            ptr = linked_ob->NVMDump(ptr, end);
            if (ptr == end) break;
            continue;
        }

        if (!item.saveToNVM)
            continue;

        if (std::holds_alternative<bool>(value)) {
            *ptr = std::get<bool>(value) ? 0 : 1;
            ptr++;
        }
        else if (std::holds_alternative<int>(value)) {
            // convert to char array
            int val = std::get<int>(value);
            size_t size = sizeof(int);

            char bytes[size];
            memcpy(bytes,&val,size);

            for (size_t i = 0; i < size; i++) {
                *ptr = bytes[i];
                ptr++;
                if (ptr == end) break;
            }
        }
        else if (std::holds_alternative<std::string>(value)) {
            const std::string &str = std::get<std::string>(value);
            size_t len = str.length();
            if (len > item.NVMStringLen) {
                len = item.NVMStringLen;
            }
            for (char c : str) {
                *ptr++ = static_cast<unsigned char>(c);
                if (ptr == end) break;
            }
            // if not use full length, add \0 at the end
            if (len < item.NVMStringLen) {
                *ptr++ = 0;
                if (ptr == end) break;
            }
        }
    }
    return ptr;
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

    ObjectModelSetterReturn ret = (*it->second.setter)(label,value);
    if (ret == ObjectModelSetterReturn::OK) {
        updateLocalProperty(label,value);
    }

    return ret;
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
            if (watcher.property == it->first && watcher.functor) {
                (*watcher.functor)(watcher.property,it->second);
            }
        }
    }
}