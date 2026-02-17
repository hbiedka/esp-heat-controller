#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include "ObjectModelCommon.h"
#include "ObjectModelWatcher.h"

using WatcherMap = std::vector<ObjectModelWatcherItem>;

class ObjectModel {
    private:
        WatcherMap watchers;
        
    protected:
        ObjectModelItemMap omItems;
        void updateLocalProperty(const std::string &label, const ObjectModelItemValue &value);
    public:
        virtual ObjectModelItemMap &getObjectModel(){ return omItems; };
        std::string serialize();
        ObjectModelGetterReturn getProperty(const std::string &label, ObjectModelItemValue &value);
        ObjectModelSetterReturn setProperty(const std::string &label, const ObjectModelItemValue &value);

        ObjectModelGetterReturn getBool(const std::string &label, bool &value);
        ObjectModelGetterReturn getInt(const std::string &label, int &value);
        ObjectModelGetterReturn getString(const std::string &label, std::string &value);

        void AddWatcher(const std::string &property, ObjectModelWatcher *watcher) {
            watchers.push_back({property,watcher});
        };
        void RemoveWatcher(std::string &property, ObjectModelWatcher &watcher) {
            //TODO
        };

};

class ObjectModelList : public ObjectModel {
    public:
        ObjectModelList(const std::vector<ObjectModel*> &children) {
            for (size_t i = 0; i < children.size(); i++) {
                omItems[std::to_string(i)] = ObjectModelItem{children[i]};
            }
        }
        ObjectModel* operator[](size_t index) {
            auto it = omItems.find(std::to_string(index));
            if (it != omItems.end() && std::holds_alternative<ObjectModel*>(it->second.value)) {
                return std::get<ObjectModel*>(it->second.value);
            }
            return nullptr;
        }
};

#endif 