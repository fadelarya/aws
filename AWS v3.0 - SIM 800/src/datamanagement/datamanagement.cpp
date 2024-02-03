#include "datamanagement.h"
StaticJsonDocument<200> datasensor;
StaticJsonDocument<200> datarssi;

File myfile;

datamanagement::datamanagement(int id)
{
 this->id2 = id;
}

float datamanagement::readCalibration(const char *path)
{
  String var = readFile(SD,path);
  float var_converted = var.toFloat();
  delay(100);
  return var_converted;
}

int datamanagement::readSleep(const char *path)
{
  String var = readFile(SD,path);
  int var_converted = var.toInt();
  delay(100);
  return var_converted;
}

String datamanagement::readApiKey(const char *path)
{
  String var = readFile(SD,path);
  delay(100);
  return var;
}

void datamanagement::saveSignalQuality(int rssiPercentage)
{
  String buf_quality = String(rssiPercentage);
  Serial.println(buf_quality);
  datarssi["rssi"] = rssiPercentage;
  // saveJsonToSD(SD,"/datadisplay/data2.json",datarssi);
  // writeFile(SD, "/RSSI.txt",buf_quality.c_str());
}

void datamanagement::start()
{
    Serial.println("SD Card Test");
    if (!SD.begin()) {
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    myfile.close();    
    String bangun = "Sudah bangun dari sleep\n";
    appendFile(SD, "/log.txt", bangun.c_str());
}

void datamanagement::saveJsonToSD(fs::FS &fs, const char *path, JsonDocument& jsonDoc)
{
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  File dataFile = SD.open(path, FILE_WRITE); // Use FILE_WRITE mode
  if (dataFile) {
    dataFile.print(jsonString);
    dataFile.close();
  } else {
    Serial.println("Error opening data file");
  }
}

void datamanagement::saveSensor(float result[]) {
  // String data_voltage = String(result[0]);
  // String data_power = String(result[3]);
  // String temp = String(result[4]);
  // String humidity = String(result[5]);
  // String atm_press = String(result[2]);
  // String wind_speed = String(result[7]);
  // String wind_dir = String(result[8]);
  // String rainfall = String(result[6]);
  // String radiance = String(result[1]);
  // String pm25 = String(result[9]);
  // String pm10 = String(result[10]);

  datasensor["temperature"] = result[4];
  datasensor["humidity"] = result[5];
  datasensor["atmpress"] = result[2];
  datasensor["windspeed"] = result[7];
  datasensor["winddir"] = result[8];
  datasensor["rainfall"] = result[6];
  datasensor["radiance"] = result[1];
  datasensor["pm25"] = result[9];
  datasensor["pm10"] = result[10];
  datasensor["voltage"] = result[0];
  datasensor["daya"] = result[3];
  
  saveJsonToSD(SD,"/datadisplay/data1.json",datasensor);

  // writeFile(SD,"/SENS1.txt", temp.c_str());
  // writeFile(SD,"/SENS2.txt", humidity.c_str());
  // writeFile(SD,"/SENS3.txt", atm_press.c_str());
  // writeFile(SD,"/SENS4.txt", wind_speed.c_str());
  // writeFile(SD,"/SENS5.txt", wind_dir.c_str());
  // writeFile(SD,"/SENS6.txt", rainfall.c_str());
  // writeFile(SD,"/SENS7.txt", radiance.c_str());
  // writeFile(SD,"/SENS8.txt", pm25.c_str());
  // writeFile(SD,"/SENS9.txt", pm10.c_str());

  // writeFile(SD,"/VOLTAGE.txt", data_voltage.c_str());
  // writeFile(SD,"/DAYA.txt", data_power.c_str());
}

void datamanagement::saveDataLost(String tanggal2, String waktu, float result[]){
  String dataMessage = "&D=" + tanggal2 + "&T=" + waktu + "&C0=" + String(result[0]) + "&C1=" + 
                String(result[1]) + "&C2=" + String(result[2]) + "&C3=" + String(result[3]) + 
                "&C4=" + String(result[4]) + "&C5=" + String(result[5]) + "&C6=" + String(result[6]) 
                + "&C7=" + String(result[7]) + "&C8=" + String(result[8]) + "&C9=" + String(result[9]) + "&C10=" + String(result[10]) + "&R=3" + '\n';
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/datasensor.txt", dataMessage.c_str());
}

void datamanagement::checkFile(const char *path){
  myfile = SD.open(path);
  if(!myfile) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    myfile = SD.open(path,FILE_WRITE);
  }
  else {
    Serial.println("File already exists");  
  }
  myfile.close();
}

String datamanagement::readFile(fs::FS &fs, const char *path){
    // Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        Serial.println("- empty file or failed to open file");
        return String();
    }
    // Serial.println("- read from file:");
    String fileContent;
    while (file.available())
    {
        fileContent += String((char)file.read());
    }
    file.close();
    // Serial.println(fileContent);
    return fileContent;
}

void datamanagement::writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);
    File file = fs.open(path, "w");
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

void datamanagement::appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File myfile = fs.open(path, FILE_APPEND);
  if(!myfile) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(myfile.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  myfile.close();
}