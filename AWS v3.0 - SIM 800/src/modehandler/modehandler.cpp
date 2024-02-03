#include "modehandler.h"

#include <WiFiClient.h>
#include <uRTCLib.h>
#include "datamanagement/datamanagement.h"

WiFiClient client3;
datamanagement data3(8);

modehandler::modehandler(int _buttonPin, int _ledPin)
{
    this->buttonPin = _buttonPin;
    this->ledPin = _ledPin;
    this->isMaintenance = false;
}

void modehandler::setupButton()
{
    pinMode(this->buttonPin, INPUT);
    pinMode(this->ledPin, OUTPUT);
}

bool modehandler::getMaintenanceState(bool update)
{
    if (update)
    {
        int buttonState = analogRead(this->buttonPin);
        
        if (buttonState == LOW)
        {
            this->isMaintenance = true;
            digitalWrite(this->ledPin, HIGH);
            Serial.println("Maintanance State is True!");
        }
        else
        {
            this->isMaintenance = false;
            digitalWrite(this->ledPin, LOW);
            Serial.println("Maintanance State is False!");
        }
    }

    return this->isMaintenance;
}

void modehandler::chooseWifiSetting(const char *ssid, const char *password)
{
    if (this->isMaintenance)
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
        WiFi.softAP(ssid, password);
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
    }
}

void modehandler::tryConnecting(const char *SSID, const char *PWD)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    String tidakConnect = "Tidak tersambung dengan WiFi\n";
    data3.appendFile(SD, "/log.txt", tidakConnect.c_str());
    Serial.println("Wifi disconnected, reconnecting...");
    WiFi.disconnect();
    delay(500);
    Serial.println("Mencoba connect ulang...");
    WiFi.begin(SSID,PWD);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.println("Connecting to WiFi...");
      counter++;
      if(counter == 10) {
        break;
      }
    }
  
  }

}

int modehandler::mapRSSItoPercentage(int rssi, int rssiMin, int rssiMax){
  int percentage = map(rssi, rssiMin, rssiMax, 0, 100);
  percentage = constrain(percentage, 0, 100);
  return percentage;
}
