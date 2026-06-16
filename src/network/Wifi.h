#ifndef WIFI_H
#define WIFI_H


#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "objectModel/ObjectModel.h"

class WifiEngine;

class SsidAndPasswdSetter : public ObjectModelSetter {
    private:
        WifiEngine &wifi;
    public:
        SsidAndPasswdSetter(WifiEngine &_wifi);
        ObjectModelSetterReturn operator()(const std::string &label, const ObjectModelItemValue &value) override;
};


enum class WifiState {
    DISCONNECTED = 0,
    START_CONNECT,
    CONNECTING,
    CONNECTED
};

class WifiEngine : public ObjectModel
{
    private:
        WifiState state = WifiState::DISCONNECTED;
        unsigned long suspenseTs = 0;
        unsigned long timeoutTs = 0;
    
        void TryConnect();

    public:
        friend class SsidAndPasswdSetter;
        SsidAndPasswdSetter ssidAndPasswdSetterFunctor;

        WifiEngine();
        void ShowAvailableNetworks();
        void Spin(unsigned long ts);
};

#endif