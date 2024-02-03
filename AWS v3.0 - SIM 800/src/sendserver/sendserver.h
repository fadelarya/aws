#ifndef SENDSERVER_H
#define SENDSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include <ArduinoJson.h>
#include <SD.h>

t_httpUpdate_return UpdateFirmware(String url_update);

class sendserver{
    public:
        sendserver(const char *server, const char *server3, String keyyy, String key, const char *server2);
        void trySending(float data[], String tanggal2, String waktu, float firmware, uint64_t freeSize);
        void quickSend(float data[]);
        void kirim(String tanggal2, String waktu, float data[], float firmware, uint64_t freeSize);
        int kirim1(float data[], float firmware, uint64_t freeSize);
        int kirim2(float data[], float firmware, uint64_t freeSize);
        void kirim3(String tanggal2, String waktu, float data[], float firmware, uint64_t freeSize);
        void kirim_datalost(String buffer);
        void cekUpdate(String url, String type, double firmware_version);
        String getFieldFromJson(String payload, const char* fieldName);
        void connect_quicksend(const char *SSID, const char *PWD);
        void initSIM();
        void cekWaktu();
        float *result;
        int statekirim, state, state2, state3;
        String buffer2, key_sipasi, key_agrieye;
        const char *server_sipasi;
        const char *server_connect;
        const char *server_agrieye;
};

#endif