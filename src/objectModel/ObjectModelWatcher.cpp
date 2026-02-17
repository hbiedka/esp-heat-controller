#include "ObjectModelWatcher.h"
#include "ObjectModel.h"

DebugObjectModelWatcher::DebugObjectModelWatcher(const std::string &_label) :
    label(_label)
{}

void DebugObjectModelWatcher::operator()(const std::string &property, const ObjectModelItem &item)
{
    Serial.printf("%s:%s -> %s\n",
        label.c_str(),
        property.c_str(),
        serializeValue(item.value).c_str()
    );
}