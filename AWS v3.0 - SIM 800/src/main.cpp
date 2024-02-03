#include <Arduino.h>
#include <declare.h>
#include <uRTCLib.h>
#include <esp_task_wdt.h>
#include <ESPDash.h>
#include <ESPAsyncWebServer.h>
#include "akuisisidata/akuisisidata.h"
#include "modehandler/modehandler.h"
#include "sendserver/sendserver.h"
#include "datamanagement/datamanagement.h"

#define FIRMWARE_VERSION 3.0
#define TYPE "pro"
// #define UPDATE_URL "https://fadelaryap.github.io/ujicoba.github.io/firmware.json"

uRTCLib rtc(0x68);
AsyncWebServer serverService(80);
ESPDash dashboard(&serverService, true);

akuisisi sensor(SSRPin);
modehandler mode(buttonPin, ledPin);
sendserver send(server,server3,keyyy,key,server2);
datamanagement data(6);

void readConfig();
int readJsonInt(fs::FS &fs, const char *path, const char *variable);
float readJsonFloat(fs::FS &fs, const char *path, const char *variable);
String readJsonString(fs::FS &fs, const char *path, const char *variable);

void setup() {
  Serial.begin(9600);
  sensor.setupSensor();
  mode.setupButton();

  // Wire.begin();
  URTCLIB_WIRE.begin();
  data.start();
  
  readConfig();
  nama = readJsonString(SD,"/datadisplay/device.json","name");

  DynamicJsonDocument servJson(1024); 
  cardSize = SD.totalBytes() / (1024 * 1024);
  usedSize = SD.usedBytes() / (1024 * 1024);
  freeSize = cardSize - usedSize;

  servJson["name"] = nama;
  servJson["firmware"] = FIRMWARE_VERSION;
  servJson["storage_use"] = usedSize;
  servJson["storage_all"] = cardSize;

  String jsonConfig;
  serializeJson(servJson, jsonConfig);
  data.writeFile(SD, "/datadisplay/device.json", jsonConfig.c_str());
  Serial.println("Maintanance button will be read in 5 seconds");
  delay(5000);
  maintenanceState = mode.getMaintenanceState(true);
  send.initSIM();
  if(maintenanceState)
  {
    mode.chooseWifiSetting(SSID_AP, PWD_AP);
    serverService.begin();
    sensor.turnOffSensor();
  } else
  {
    sensor.setupBegin();
    sensor.turnOnSensor();
    send.cekWaktu();
  }
  
  Serial.println("Connected ");
  Serial.println("Modbus RTU Master Online");
   
  esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);
}

void loop() {
  esp_task_wdt_reset();

  if(!maintenanceState){
    rtc.refresh();
    Serial.print(String(rtc.hour())); Serial.print(":"); Serial.print(String(rtc.minute())); Serial.print(":"); Serial.println(String(rtc.second()));
    uint8_t bagi = rtc.minute() % 10;
    uint8_t detik2 = rtc.second();

    if((bagi != 0) && (detik2 != 0)){
      delay(1000);
      return;
    } else if ((bagi == 0) && (detik2 == 0))
    {
      int tahuun = rtc.year();int bulaan = rtc.month(); int haari = rtc.day();
      String bulan;
      if(bulaan == 1){bulan = "Jan";}else if(bulaan==2){bulan="Feb";}else if(bulaan == 3){bulan = "Mar";}else if(bulaan==4){bulan="Apr";}else if(bulaan == 5)
        {bulan = "May";}else if(bulaan==6){bulan="Jun";}else if(bulaan == 7){bulan = "Jul";}else if(bulaan==8){bulan="Aug";}else if(bulaan == 9){bulan = "Sep";}
        else if(bulaan==10){bulan="Oct";}else if(bulaan == 11){bulan = "Nov";}else if(bulaan==12){bulan="Dec";}
      String tanggal3 = String(haari) + '-' + String(bulan) + "-20" + String(tahuun);
      String waktu2 = String(rtc.hour()) + ':' + String(rtc.minute());

      tanggal = String(rtc.year())+ '-' +String(rtc.month())+'-'+String(rtc.day());
      tanggal2 = "20" + String(rtc.year())+ '-' +String(rtc.month())+'-'+String(rtc.day());
      waktu = String(rtc.hour())+':'+String(rtc.minute())+':'+String(rtc.second());

      String timestmp = tanggal2 + ' ' + waktu + "\n";
      data.appendFile(SD, "/log.txt", timestmp.c_str());

      String msg = "Masuk mode GPRS\n";
      data.appendFile(SD, "/log.txt", msg.c_str());

      result = sensor.get_all();
      delay(3000);
      sensor.turnOffSensor();
      
      String nilaisave2 = sensor.print(result);
      data.appendFile(SD, "/log.txt", nilaisave2.c_str());
      data.saveSensor(result);

      data.checkFile("/dataraw.csv");
      String jadicsv = String(tanggal3) + ',' + String(waktu2) + ',' + String(result[0]) +',' + String(result[1]) +',' + String(result[2]) + ',' + String(result[3]) + ',' + String(result[4]) + ',' + String(result[5]) + ',' + String(result[6]) + ',' + String(result[7]) + ',' + String(result[8]) + ',' + String(result[9]) + ',' + String(result[10]) +"\n";
      data.appendFile(SD, "/dataraw.csv", jadicsv.c_str());

      Serial.println("Mencoba Connect");
      Serial.println(keyyy);

      float firmware = FIRMWARE_VERSION;
      send.trySending(result, tanggal2, waktu, firmware, freeSize);
      sensor.print_all(result);
    
      // esp_sleep_enable_timer_wakeup(pengkali * 270); //Kedung Putri

      esp_sleep_enable_timer_wakeup(pengkali * durasisleep);
      String mulaisleep = "Mulai sleep...\n\n";
      data.appendFile(SD, "/log.txt", mulaisleep.c_str());
      Serial.println("Going to sleep now");
      delay(1000);
      Serial.flush(); 
      esp_deep_sleep_start();
    }
  } else
  {
    delay(5000);
    // data.checkFile("/configuration/quicksend.txt");
    String quicksend = data.readFile(SD, "/configuration/quicksend.txt");
    if(quicksend == "1"){
      send.connect_quicksend(SSID,PWD);
      sensor.setupBegin();
      sensor.turnOnSensor();
      Serial.println("Menyalakan sensor selama 10 detik....");
      delay(10000);
      Serial.println("Membaca sensor...");
      result = sensor.get_all();
      delay(3000);
      sensor.turnOffSensor();
      send.quickSend(result);
    }

    // // Get and print RSSI
    // int rssi = WiFi.RSSI();
    // // Convert RSSI to percentage based on your observed range
    // rssiPercentage = mode.mapRSSItoPercentage(rssi, -100, -30);
    // data.saveSignalQuality(rssiPercentage);
  }
}

void readConfig()
{
  File file = SD.open("/configuration/config.json");
  DynamicJsonDocument doc(1024);

  deserializeJson(doc, file);
  durasisleep = doc["durasisleep"];
  password = doc["password"].as<String>();

  file.close();
  
  a_temp = data.readCalibration("/calibration/a_1.txt");
  c_temp = data.readCalibration("/calibration/c_1.txt");

  a_humidity = data.readCalibration("/calibration/a_2.txt");
  c_humidity = data.readCalibration("/calibration/c_2.txt");

  a_atm_press = data.readCalibration("/calibration/a_3.txt");
  c_atm_press = data.readCalibration("/calibration/c_3.txt");

  a_wind_speed = data.readCalibration("/calibration/a_4.txt");
  c_wind_speed = data.readCalibration("/calibration/c_4.txt");

  a_wind_dir = data.readCalibration("/calibration/a_5.txt");
  c_wind_dir = data.readCalibration("/calibration/c_5.txt");

  a_rainfall = data.readCalibration("/calibration/a_6.txt");
  c_rainfall = data.readCalibration("/calibration/c_6.txt");

  a_radiance = data.readCalibration("/calibration/a_7.txt");
  c_radiance = data.readCalibration("/calibration/c_7.txt");

  a_pm25 = data.readCalibration("/calibration/a_8.txt");
  c_pm25 = data.readCalibration("/calibration/c_8.txt");

  a_pm10 = data.readCalibration("/calibration/a_9.txt");
  c_pm10 = data.readCalibration("/calibration/c_9.txt");

  File myfile = SD.open("/configuration/server.json");
  DynamicJsonDocument serverJson(1024);

  deserializeJson(serverJson, myfile);
  keyyy = serverJson["apiKey"].as<String>();
  Serial.println(keyyy);
  key = serverJson["apiKey2"].as<String>();
  Serial.println(key);

  file.close();
}

String readJsonString(fs::FS &fs, const char *path, const char *variable){
  File file = fs.open(path);
  DynamicJsonDocument docc(1024);

  deserializeJson(docc, file);
  
  String cek = docc[variable].as<String>();
  file.close();
  delay(500);
  return cek;
}