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
            //validate ssid
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
            //validate passwd
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
        {"passwd", ObjectModelItem::createSecretStringItem("", &ssidAndPasswdSetterFunctor, 64)},
        {"state", ObjectModelItem{ ObjectModelItemValue{0} }},
        {"ip" , ObjectModelItem{ ObjectModelItemValue{std::string("0.0.0.0")} }}
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
                // TODO run hotspot mode
                state = WifiState::DISCONNECTED;
            }
            else if (status == WL_CONNECTED) {
                Serial.println("Wifi: connected. IP: "+WiFi.localIP().toString());
                updateLocalProperty("ip", std::string(WiFi.localIP().toString().c_str()));
                state = WifiState::CONNECTED;
            }
            suspenseTs = ts + 500;
            break;

        case WifiState::CONNECTED:
            if (status != WL_CONNECTED) {
                Serial.println("Wifi: disconnected");
                state = WifiState::DISCONNECTED;
                suspenseTs = ts + 1000;
                break;
            }

            suspenseTs = ts + 10000;
            break;
    }

    //update Object Model
    updateLocalProperty("state", static_cast<int>(state));
}