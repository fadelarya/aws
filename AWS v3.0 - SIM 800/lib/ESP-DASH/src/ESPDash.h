/*
____ ____ ___  ___  ____ ____ _  _ 
|___ [__  |__] |  \ |__| [__  |__| 
|___ ___] |    |__/ |  | ___] |  | 
                                   
ESP-DASH V3
---------------------
Author: Ayush Sharma
First Commit: Nov 5, 2017
Github URL: https://github.com/ayushsharma82/ESP-DASH

*/

#ifndef ESPDash_h
#define ESPDash_h

#include <functional>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Arduino.h"
#include "stdlib_noniso.h"
// #include "webpage.h"
#include "HtmlHandling.h"
#include "ESPAsyncWebServer.h"


#if defined(ESP8266)
    #define HARDWARE "ESP8266"
    #include "ESP8266WiFi.h"
    #include "ESPAsyncTCP.h"
#elif defined(ESP32)
    #define HARDWARE "ESP32"
    #include "WiFi.h"
    #include "AsyncTCP.h"
#endif

#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


// ESPDASH Class
class ESPDash{
  private:
    AsyncWebServer* _server = nullptr;
    AsyncWebSocket* _ws = nullptr;

  public:

    ESPDash(AsyncWebServer* server, bool enable_stats = true);

    HtmlHandling *htmlHandler{};
    
};

#endif
