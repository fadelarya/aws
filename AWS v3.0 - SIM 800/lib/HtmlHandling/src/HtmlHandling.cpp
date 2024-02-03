#include "HtmlHandling.h"

String HtmlHandling::processor(const String &var)
{
    if (var == "endpoint")
    {
        return readFile(SD, "/configuration/endpoint.txt");
    }
    else if (var == "api_key")
    {
        return readFile(SD, "/configuration/api_key.txt");
    }
    if (var == "durasisleep")
    {
        return readFile(SD, "/configuration/durasi_sleep.txt");
    }
    else if (var == "password")
    {
        return readFile(SD, "/configuration/pass_word.txt");
    }
    return String();
}


String HtmlHandling::readSens1(){
  return readFile(SD, "/SENS1.txt");
}

String HtmlHandling::readSens2(){
  return readFile(SD, "/SENS2.txt");
}

String HtmlHandling::readSens3(){
  return readFile(SD, "/SENS3.txt");
}

String HtmlHandling::readSens4(){
  return readFile(SD, "/SENS4.txt");
}

String HtmlHandling::readSens5(){
  return readFile(SD, "/SENS5.txt");
}

String HtmlHandling::readSens6(){
  return readFile(SD, "/SENS6.txt");
}

String HtmlHandling::readSens7(){
  return readFile(SD, "/SENS7.txt");
}

String HtmlHandling::readSens8(){
  return readFile(SD, "/SENS8.txt");
}

String HtmlHandling::readSens9(){
  return readFile(SD, "/SENS9.txt");
}

String HtmlHandling::readVolt(){
  return readFile(SD, "/VOLTAGE.txt");
}

String HtmlHandling::readDAYA(){
  return readFile(SD, "/DAYA.txt");
}

String HtmlHandling::readRSSI(){
  return readFile(SD, "/RSSI.txt");
}

String HtmlHandling::readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        Serial.println("- empty file or failed to open file");
        return String();
    }
    Serial.println("- read from file:");
    String fileContent;
    while (file.available())
    {
        fileContent += String((char)file.read());
    }
    Serial.println(fileContent);
    file.close();
    return fileContent;
}

void HtmlHandling::cek(){
    int a = 0;
}

void HtmlHandling::writeFile(fs::FS &fs, const char *path, const char *message)
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

void HtmlHandling::hapusFile(){

  SD.remove("/log.txt");
}