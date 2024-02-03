
#ifndef HtmlHandling
#define ESPDash_h
#include "FS.h"
#include <SD.h>
#include <SPI.h>
#include "Arduino.h"

#if defined(ESP8266)
    #include <Hash.h>
    #include <FS.h>
#elif defined(ESP32)
    #include <SPIFFS.h>
#endif


class HtmlHandling{
    public:
        static String processor(const String& var);
        static String readFile(fs::FS &fs, const char * path);
        static void writeFile(fs::FS &fs, const char * path, const char * message);
        static String readSens1();
        static String readSens2();
        static String readSens3();
        static String readSens4();
        static String readSens5();
        static String readSens6();
        static String readSens7();
        static String readSens8();
        static String readSens9();
        static String readRSSI();
        static String readDAYA();
        static String readVolt();
        void cek();
        void hapusFile();
};

#endif