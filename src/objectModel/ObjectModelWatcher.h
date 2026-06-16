#ifndef OBJECT_MODEL_WATCHER_H
#define OBJECT_MODEL_WATCHER_H

#include <Arduino.h>
#include "ObjectModelCommon.h"

struct ObjectModelWatcherItem {
    std::string property;
    std::string context;
    ObjectModelWatcher *functor = nullptr;
};

class ObjectModelWatcher {
    public:
        virtual ~ObjectModelWatcher() = default;
        virtual void operator()(const std::string &property, const ObjectModelItem &item) = 0;
        virtual void operator()(const std::string &property, const std::string &context, const ObjectModelItem &item) {
            // default implementation ignores context, can be overridden by derived classes if needed
            operator()(property, item);
        }
};

class DebugObjectModelWatcher : public ObjectModelWatcher {
    private:
        std::string label;
    public:
        DebugObjectModelWatcher(const std::string &_label);
        void operator()(const std::string &property, const ObjectModelItem &item) override;
};

#endif
