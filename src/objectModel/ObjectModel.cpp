#include "ObjectModel.h"

std::string ObjectModel::serialize() {
    ObjectModelItemList &om = getObjectModel();

    std::string result = "{";
    
    for (size_t i = 0; i < om.size(); i++) {
        const auto& item = om[i];
        
        // Add comma if not first item
        if (i > 0) {
            result += ",";
        }
        
        // Add property name
        result += "\"" + item.label + "\":";
        
        // Add value based on type
        switch (item.type) {
            case ObjectModelItemType::BOOL:
                //check if the variant is a boolean
                if (std::holds_alternative<bool>(item.value)) {
                    result += std::get<bool>(item.value) ? "true" : "false";
                } else {
                    //TODO throw exception or handle error
                    result += "null"; // Handle unexpected type
                }
                break;
                
            case ObjectModelItemType::INT:
                if (std::holds_alternative<int>(item.value)) {
                    result += std::to_string(std::get<int>(item.value));
                } else {
                    result += "null";
                }
                break;
                
            case ObjectModelItemType::STRING:
                if (std::holds_alternative<std::string>(item.value)) {
                    // Assuming std::string is a class with a method to get the C-string representation
                    result += "\"" + std::get<std::string>(item.value) + "\""; // Uncomment if std::string has a method to get C-string
                } else {
                    result += "null";
                }
                break;
                
            case ObjectModelItemType::LINK:
                if (std::holds_alternative<ObjectModel*>(item.value)) {
                    ObjectModel *linked_ob = std::get<ObjectModel*>(item.value);
                    if (linked_ob != nullptr)
                        result += linked_ob->serialize();
                    else
                        result += "{}";
                } else {
                    result += "null";
                }
                break;
        }
    }
    
    result += "}";
    return result;
}

int ObjectModel::getIndexFromLabel(const std::string &label) {
    auto it = std::find_if(omItems.begin(), omItems.end(),
        [label](ObjectModelItem item){
            return item.label == label;
        }
    );
    if (it == omItems.end())
        return -1;

    return std::distance(omItems.begin(),it);
}

ObjectModelSetterReturn ObjectModel::setProperty(const std::string &label, const ObjectModelItemValue &value) {
    //find index
    int index = getIndexFromLabel(label);
    if (index < 0 || (unsigned int)index >= omItems.size() )
        return ObjectModelSetterReturn::INVLABEL;

    if (omItems[index].setter == nullptr)
        return ObjectModelSetterReturn::READONLY;

    return (*omItems[index].setter)(label,value);
}