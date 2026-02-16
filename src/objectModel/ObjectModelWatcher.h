#ifndef OBJECT_MODEL_WATCHER_H
#define OBJECT_MODEL_WATCHER_H

#include <Arduino.h>
#include "ObjectModelCommon.h"

struct ObjectModelWatcherItem {
    std::string id;
    std::string property;
    ObjectModelWatcher *object = nullptr;
};

class ObjectModelWatcher {
    public:
        virtual ~ObjectModelWatcher() = default;
        virtual void Watch(ObjectModel *om, const std::string &property, const std::string &id);
        virtual void WatchCallback(const std::string &id, const ObjectModelItem &item) = 0;
};

class DebugObjectModelWatcher : public ObjectModelWatcher {
    public:
        void WatchCallback(const std::string &id, const ObjectModelItem &item);
};

#endif
