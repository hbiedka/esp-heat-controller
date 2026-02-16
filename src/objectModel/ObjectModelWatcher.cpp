#include "ObjectModelWatcher.h"
#include "ObjectModel.h"

void ObjectModelWatcher::Watch(ObjectModel *om, const std::string &property, const std::string &id) {
    if (!om) return; //TODO throw exception

    om->AddWatcher(id,property,this);
}

void DebugObjectModelWatcher::WatchCallback(const std::string &id, const ObjectModelItem &item)
{
    Serial.printf("%s -> %s\n",
        id.c_str(),
        serializeValue(item.value).c_str()
    );
}