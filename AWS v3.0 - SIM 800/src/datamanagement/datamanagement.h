#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <Arduino.h>
#include <SD.h>
#include "FS.h"
#include <ArduinoJson.h>

class datamanagement{
    public:
        datamanagement(int id);
        void start();
        float readCalibration(const char *path);
        int readSleep(const char *path);
        String readApiKey(const char *path);
        void saveJsonToSD(fs::FS &fs, const char * path, JsonDocument& jsonDoc);
        void saveSensor(float result[]);
        void saveDataLost(String tanggal2, String waktu, float result[]);
        void checkFile(const char * path);
        String readFile(fs::FS &fs, const char *path);
        void writeFile(fs::FS &fs, const char * path, const char * message);
        void appendFile(fs::FS &fs, const char * path, const char * message);
        void saveSignalQuality(int rssiPercentage);
        int id2;
};

#endif