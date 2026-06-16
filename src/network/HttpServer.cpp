#include "HttpServer.h"

HttpServer::HttpServer(ObjectModel &root) :
    omRoot(root)
{};

void HttpServer::Spin(unsigned long ts) {
    if (!initialized) {
        begin();
    }
    server.handleClient();
}

void HttpServer::handleObjectModelRoot() {
    // TODO consider sending in chunks if response is too big
    Serial.println("RQ from " + server.client().remoteIP().toString() + ": " + server.uri());
    std::string response = omRoot.serialize();
    server.send(200, "application/json", response.c_str());
}

void HttpServer::handleNotFound() {
    server.send(404, "text/plain", "Not found");
}
void HttpServer::begin() {
    server.on("/api/object-model", std::bind(&HttpServer::handleObjectModelRoot, this));
    server.onNotFound(std::bind(&HttpServer::handleNotFound, this));
    server.begin();
    initialized = true;
}
