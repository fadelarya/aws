#ifndef MODEHANDLER_H
#define MODEHANDLER_H

#include <Arduino.h>
#include <WiFi.h>


class modehandler{
    public:
        modehandler(int _buttonPin, int _ledPin);
        void setupButton();
        bool getMaintenanceState(bool update);
        void chooseWifiSetting(const char *ssid, const char *password);
        void tryConnecting(const char *SSID, const char *PWD);
        int mapRSSItoPercentage(int rssi, int rssiMin, int rssiMax);
        

        int year, month, day, hour, minute, second, rssiPercentage;
        int buttonPin, ledPin;
        bool isMaintenance; 
};


#endif