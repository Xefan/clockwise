#pragma once

#include "ImprovWiFiLibrary.h"
#include <WiFi.h>
#include "CWWebServer.h"
#include "StatusController.h"

ImprovWiFi improvSerial(&Serial);

struct WiFiController
{
  static void onImprovWiFiErrorCb(ImprovTypes::Error err)
  {
    ClockwiseWebServer::getInstance()->stopWebServer();
    StatusController::getInstance()->blink_led(2000, 3); 
  }

  static void onImprovWiFiConnectedCb(const char* ssid, const char* password)
  {
    ClockwiseParams::getInstance()->load();
    ClockwiseParams::getInstance()->wifiSsid = String(ssid);
    ClockwiseParams::getInstance()->wifiPwd = String(password);
    ClockwiseParams::getInstance()->save();

    ClockwiseWebServer::getInstance()->startWebServer();
  }

  static bool isConnected() {
    return improvSerial.isConnected();
  }

  static void handleImprovWiFi()
  {
    improvSerial.handleSerial();
  }

  void begin()
  {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32, "CW-20230324", "1.1.0", "Clockwise");
    improvSerial.onImprovError(onImprovWiFiErrorCb);
    improvSerial.onImprovConnected(onImprovWiFiConnectedCb);

    ClockwiseParams::getInstance()->load();

    if (!ClockwiseParams::getInstance()->wifiSsid.isEmpty()) {
      if (improvSerial.tryConnectToWifi(ClockwiseParams::getInstance()->wifiSsid.c_str(), ClockwiseParams::getInstance()->wifiPwd.c_str()))
      {
        ClockwiseWebServer::getInstance()->startWebServer();
      }
    }
  }

  
};
