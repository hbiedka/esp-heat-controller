#ifndef OBJECT_MODEL_WATCHER_H
#define OBJECT_MODEL_WATCHER_H

#include <Arduino.h>
#include "ObjectModelCommon.h"

struct ObjectModelWatcherItem {
    std::string property;
    ObjectModelWatcher *functor = nullptr;
};

class ObjectModelWatcher {
    public:
        virtual ~ObjectModelWatcher() = default;
        virtual void operator()(const std::string &property, const ObjectModelItem &item) = 0;
};

class DebugObjectModelWatcher : public ObjectModelWatcher {
    private:
        std::string label;
    public:
        DebugObjectModelWatcher(const std::string &_label);
        void operator()(const std::string &property, const ObjectModelItem &item);
};

#endif
