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
                if (item.link != nullptr) {
                    // Assuming ObjectModel has a method to serialize itself
                    // result += item.link->serialize(); // Uncomment if ObjectModel has a serialize method
                    result += "link"; // Placeholder for link serialization
                } else {
                    result += "null";
                }
                break;
        }
    }
    
    result += "}";
    return result;
}
