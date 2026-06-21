#include "HttpServer.h"
#include <LittleFS.h>

HttpServer::HttpServer(ObjectModel &root) :
    omRoot(root)
{};

void HttpServer::Spin(unsigned long ts) {
    if (!initialized) {
        begin();
    }
    server.handleClient();
}

void HttpServer::handleFileFromFS() {
    String path = server.uri();

    Serial.print("RQ from " + server.client().remoteIP().toString() + ": " + path);

    if (path == "/") {
        path += "/index.html";
    }

    String contentType = "text/plain";
    if (path.endsWith(".html")) contentType = "text/html";
    else if (path.endsWith(".css"))  contentType = "text/css";
    else if (path.endsWith(".js"))   contentType = "application/javascript";
    else if (path.endsWith(".ico"))  contentType = "image/x-icon";

    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        server.streamFile(file, contentType);
        file.close();
        Serial.println(" 200 OK");
        return;
    }

    // here if not found
    Serial.println(" 404 Not found");
    server.send(404, "text/plain", "Not found");
}

void HttpServer::handleObjectModelRoot() {
    // TODO consider sending in chunks if response is too big
    Serial.println("RQ from " + server.client().remoteIP().toString() + ": GET " + server.uri());
    std::string response = omRoot.serialize();
    server.send(200, "application/json", response.c_str());
}

void HttpServer::handleObjectModelRootPatch() {
    Serial.println("RQ from " + server.client().remoteIP().toString() + ": PATCH" + server.uri());
    String body = server.arg("plain");
    auto result = omRoot.deserialize(body.c_str());
    if (result != ObjectModelSetterReturn::OK) {
        server.send(400, "text/plain",
            "Failed to deserialize object model patch (error code: " + String((int)result) + ")");
        return;
    }
    std::string response = omRoot.serialize();
    server.send(200, "application/json", response.c_str());

    //TODO trigger save to EEPROM
}

void HttpServer::begin() {
    if (!LittleFS.begin()) {
        Serial.println("Error mounting LittleFS");
        return;
    }

    server.on("/api/object-model", HTTP_GET, std::bind(&HttpServer::handleObjectModelRoot, this));
    server.on("/api/object-model", HTTP_PATCH, std::bind(&HttpServer::handleObjectModelRootPatch, this));
    server.onNotFound(std::bind(&HttpServer::handleFileFromFS, this));
    server.begin();
    initialized = true;
}
