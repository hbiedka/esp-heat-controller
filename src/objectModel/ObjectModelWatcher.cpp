#include "ObjectModelWatcher.h"
#include "ObjectModel.h"

void DebugObjectModelWatcher::operator()(
    const std::string &property,
    const std::string &context,
    const ObjectModelItem &item)
{
    Serial.printf("%s -> %s\n",
        context.c_str(),
        serializeValue(item.value).c_str()
    );
}