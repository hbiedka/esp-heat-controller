#include "Wifi.h"

SsidAndPasswdSetter::SsidAndPasswdSetter(WifiEngine &_wifi) :
    wifi(_wifi)
{};

ObjectModelSetterReturn SsidAndPasswdSetter::operator()(
    const std::string &label,
    const ObjectModelItemValue &value)
{
    if (std::holds_alternative<std::string>(value)) {
        std::string strValue = std::get<std::string>(value);
        if (label == "ssid") {
            //TODO validate ssid
            if (strValue.size() < 8 || strValue.size() > 32) {
                return ObjectModelSetterReturn::INVVAL;
            }
            for (auto c : strValue) {
                if (c < 32 || c > 126) {
                    return ObjectModelSetterReturn::INVVAL;
                }
            }
            return ObjectModelSetterReturn::OK;
        } else if (label == "passwd" ) {
            //TODO validate passwd
            if (strValue.size() < 8 || strValue.size() > 64) {
                return ObjectModelSetterReturn::INVVAL;
            }
            for (auto c : strValue) {
                if (c < 32 || c > 126) {
                    return ObjectModelSetterReturn::INVVAL;
                }
            }
            return ObjectModelSetterReturn::OK;
        } else {
            return ObjectModelSetterReturn::INVLABEL;
        }
    }
    return ObjectModelSetterReturn::INVTYPE;
}

WifiEngine::WifiEngine() :
    ssidAndPasswdSetterFunctor(*this)
{
    omItems = {
        {"ssid", ObjectModelItem::createStringItem("", &ssidAndPasswdSetterFunctor, 32)},
        {"passwd", ObjectModelItem::createStringItem("", &ssidAndPasswdSetterFunctor, 64)}
    };
}

void WifiEngine::TryConnect()
{
    //check if ssid and passwd are set, if so try to connect
    if (!std::holds_alternative<std::string>(omItems["ssid"].value) || !std::holds_alternative<std::string>(omItems["passwd"].value)) {
        return;
    }
    std::string ssid = std::get<std::string>(omItems["ssid"].value);
    std::string passwd = std::get<std::string>(omItems["passwd"].value);
    if (ssid.empty() ||passwd.empty())
        return;

    Serial.printf("Trying to connect to wifi with ssid '%s' and passwd '%s'\n", ssid.c_str(), passwd.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), passwd.c_str());
}

void WifiEngine::ShowAvailableNetworks()
{
    int n = WiFi.scanNetworks();
    Serial.println("Available networks:");
    for (int i = 0; i < n; ++i) {
        Serial.printf("%d: %s (%d) %s\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "encrypted");
    }
}

void WifiEngine::Spin(unsigned long ts)
{
    int timeoutInterval;
    //global suspense
    int interval = ts - suspenseTs;
    if (interval < 0) return;

    auto status = WiFi.status();
    // Serial.printf("%lu, state %d, status code %d\n", ts, (int)state, status);

    switch(state) {
        case WifiState::DISCONNECTED:
            TryConnect();
            state = WifiState::START_CONNECT;
            break;
        case WifiState::START_CONNECT:
            suspenseTs = ts + 2000;
            timeoutTs = ts + 30000;
            state = WifiState::CONNECTING;
            break;

        case WifiState::CONNECTING:

            timeoutInterval = ts - timeoutTs;
            if (timeoutInterval > 0) {
                // timeout
                Serial.println("Wifi: connection timeout");
                state = WifiState::DISCONNECTED;
            }
            else if (status == WL_CONNECTED) {
                Serial.println("Wifi: connected");
                state = WifiState::CONNECTED;
            }
            suspenseTs = ts + 500;
            break;

        case WifiState::CONNECTED:
            //TODO maintain connection

            suspenseTs = ts + 10000;
            break;
    }
}