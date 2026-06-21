#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H


#include <ESP8266WebServer.h>
#include "objectModel/ObjectModel.h"

class HttpServer {
    private:
        ESP8266WebServer server{80};
        ObjectModel &omRoot;

        bool initialized = false;

        void begin();
        void handleObjectModelRoot();
        void handleObjectModelRootPatch();
        void handleFileFromFS();
    public:
        HttpServer(ObjectModel &root);
        void Spin(unsigned long ts);
};

#endif