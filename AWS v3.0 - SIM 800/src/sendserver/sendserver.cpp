#include "sendserver.h"
#include "datamanagement/datamanagement.h"
#include <SoftwareSerial.h>
#include <uRTCLib.h>


String smt1, smt2, smt3, smt4, simpann, ccd2, cd2;
int nilaii3, nilaii5, gagal, berhasil;


WiFiClient wifi;
HTTPClient http;
datamanagement data2(7);
File myfile2;

uint8_t port = 80;    

// ----Deklarasi RX TX Soft Serial Kedung Putri---//
SoftwareSerial SIM800L(33,32);

// const char apn[]      = "internet"; //APN Telkomsel Biasa 
const char apn[]      = "m2minternet"; //APN Telkomsel IoT
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = "";

#define MODEM_TX 32
#define MODEM_RX 33
#define MODEM_RST 25
#define SerialAT Serial1

// //----Deklarasi RX TX Soft Serial Sapon---//
// SoftwareSerial SIM800L(32,33);

// const char apn[]      = "m2minternet"; 
// const char gprsUser[] = "";
// const char gprsPass[] = "";
// const char simPIN[]   = "";

// #define MODEM_TX 33
// #define MODEM_RX 32
// #define MODEM_RST 25
// #define SerialAT Serial1

//--Set type to SIM800--//
#define TINY_GSM_MODEM_SIM800 
#define TINY_GSM_RX_BUFFER   1024

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, Serial);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif


//--Deklarasi TinyGSM--//
TinyGsmClient client2(modem);

uRTCLib rtc2(0x68);

String payload, buffer, buffer3;
int delay_offline = 30000;
int nilaii1, previous_time2, cekfileada;

sendserver::sendserver(const char *server,const char *server3, String keyyy, String key, const char *server2)
{
    this->server_sipasi = server;
    this->server_agrieye = server3;
    this->key_sipasi = keyyy;
    this->key_agrieye = key;
    this->server_connect = server2;
}

void sendserver::initSIM()
{
  SIM800L.begin(9600);
  Serial.println("SIM800L GPRS Test");
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  modem.init();
}

void sendserver::cekWaktu()
{
  Serial.println("mencoba connect");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println(" fail");
  }
  else {
    Serial.println(" OK");
    Serial.println("Membaca waktu");

    int   year3    = 0;
    int   month3   = 0;
    int   day3     = 0;
    int   hour3    = 0;
    int   min3     = 0;
    int   sec3     = 0;
    float timezone = 0;

    if (modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3, &timezone)) {
      Serial.print("Year:"); Serial.print(year3); Serial.print("\tMonth:"); Serial.print(month3);  Serial.print("\tDay:"); Serial.println(day3);
      Serial.print("Hour:");Serial.print(hour3); Serial.print("\tMinute:"); Serial.print(min3); Serial.print("\tSecond:"); Serial.println(sec3);
      Serial.print("Timezone:"); Serial.println(timezone);
      int year4 = year3;
      year3 -= 2000;
      sec3 += 1;
      if(sec3 > 59){
        sec3 = 0;
        min3 += 1;
      }
      rtc2.set(sec3,min3,hour3,1,day3,month3,year3);
    }
    modem.gprsDisconnect();
    Serial.println(F("GPRS disconnected"));
  }
}

void sendserver::connect_quicksend(const char *SSID, const char *PWD)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PWD);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
    {
      // Check to see if connecting failed.
      // This is due to incorrect credentials
      if (WiFi.status() == WL_CONNECT_FAILED)
      {
          Serial.println("Failed to connect to WIFI. Please verify credentials: ");
          Serial.println();
          Serial.print("SSID: ");
          Serial.println(SSID);
          Serial.print("Password: ");
          Serial.println(PWD);
          Serial.println();
      }
      delay(1000);
    }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Hello World, I'm connected to the internets!!");
}

void sendserver::trySending(float data[], String tanggal2, String waktu, float firmware, uint64_t freeSize)
{
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println(" fail to Connect to GPRS");
    data2.saveDataLost(tanggal2, waktu, data);
    String gagalconnect = "Gagal Connect ke GRPS\n";
    data2.appendFile(SD, "/log.txt", gagalconnect.c_str());
  }
  else {
    Serial.println(" OK");
    String berhasilGPRS = "Berhasil tersambung dengan GPRS\n";
    data2.appendFile(SD, "/log.txt", berhasilGPRS.c_str());
    Serial.print("Connecting to ");
    Serial.println(this->server_sipasi);

    if (!client2.connect(this->server_connect, port)) {
      Serial.println(" fail");
      data2.saveDataLost(tanggal2, waktu, data);
      String gagalServer = "Gagal Connect ke Server\n";
      data2.appendFile(SD, "/log.txt", gagalServer.c_str());
    }

    else {
      Serial.println(" OK");
      String berhasilServer = "Berhasil Connect ke Server\n";
      data2.appendFile(SD, "/log.txt", berhasilServer.c_str());
      Serial.println("Mengirim ke cloud...");
      delay(500);
      this->statekirim = kirim1(data, firmware, freeSize);
      client2.stop();

      if (this->statekirim == 1) {
        Serial.println("11");
        if (!client2.connect(this->server_connect, port)) {
          Serial.println(" fail");
          statekirim = 2;
        } else {
          this->statekirim = kirim2(data, firmware, freeSize);
          client2.stop();
        }
      }

      if (this->statekirim == 2) {
        Serial.println("22");
        if (!client2.connect(this->server_connect, port)) {
          Serial.println(" fail");
          data2.saveDataLost(tanggal2, waktu, data);
        } else {
          kirim3(tanggal2, waktu, data, firmware, freeSize);
          client2.stop();
      }
      }

      previous_time2 = millis();
      int counter2 = 1;
      int counter = 1;
      myfile2 = SD.open("/datasensor.txt");
      while(myfile2.available()){
        
        if ((millis() - previous_time2) < delay_offline) {
          int waktudelay = delay_offline - (millis() - previous_time2);
          delay(waktudelay);
        }

        cekfileada = 1;
        
        buffer = myfile2.readStringUntil('\n');
        Serial.println(buffer);
        
        if ((counter2 == 1) || (counter2 == 2)){
          if(!client2.connect(server_agrieye, port)){
            Serial.println("Tidak bisa connect lagi");
            this->buffer2 = this->buffer2 + buffer + '\n';
            gagal++;
          }
          else {
            berhasil++;
            Serial.print("Mengirim ");
            Serial.println(buffer);
            kirim_datalost(buffer);
            client2.stop();
            previous_time2 = millis();
            buffer="";
          }
        } else {
          buffer3 = buffer3 + buffer + '\n';
        }
        
        counter2++;
        counter++;
          
      }

      Serial.println("1");
      
      myfile2.close();

      Serial.println("Removing datasensor.txt");
      SD.remove("/datasensor.txt");
      
      if((buffer3=="") && (this->buffer2!="")) {
        data2.writeFile(SD, "/datasensor.txt", this->buffer2.c_str());
      } 
      else if ((this->buffer2=="") && (buffer3!="")) {
        data2.writeFile(SD, "/datasensor.txt", buffer3.c_str());
      }
      else if ((buffer3 != "") && (this->buffer2 != "")){
        data2.writeFile(SD, "/datasensor.txt", buffer3.c_str());
        data2.appendFile(SD, "/datasensor.txt", this->buffer2.c_str());
      }
      
      if (gagal != 0) {
          String gagalConnect2 = "Gagal mengirim lagi sebanyak " + String(gagal) + " lagi\n";
          data2.appendFile(SD, "/log.txt", gagalConnect2.c_str());
      }

      if (cekfileada == 0){
        String adafile = "Masih ada file blm terkirim\n";
        data2.appendFile(SD, "/log.txt", adafile.c_str());
      }

      if (berhasil!=0){
        String berhasilConnect2 = "Berhasil Connect ke server lagi\n";
        data2.appendFile(SD, "/log.txt", berhasilConnect2.c_str());
      }
      String hapus = "File telah dihapus\n";
      data2.appendFile(SD, "/log.txt", hapus.c_str());
      modem.gprsDisconnect();
      Serial.println(F("GPRS disconnected"));
    }
  }
}

void sendserver::quickSend(float data[])
{
 String quick = "Quicksend\n";
 data2.appendFile(SD,"/log.txt",quick.c_str());
 
 String req = "&C0=" + String(data[0]) + "&C1=" + 
              String(data[1]) + "&C2=" + String(data[2]) + "&C3=" + String(data[3]) 
              + "&C4=0.00&C5=" + String(data[4]) + "&C6=" + String(data[5]) + "&C7=" 
              + String(data[6]) + "&C8=" + String(data[7]) + "&C9=" + String(data[8]) 
              + "&C10=" + String(data[9]) + "&C11=" + String(data[10]);
  Serial.println(req);
  Serial.println("Performing HTTP POST request...");

  client2.print("GET ");
  // String buatkey = "/feed/api2.php?K=" + String(key) + String(requestData);
  // client2.print(String("GET ") + buatkey + " HTTP/1.0\r\n");
  client2.print("/?key=");
  client2.print(this->key_sipasi);
  client2.print(req);
  client2.print(" HTTP/1.0\n");
  // client2.print(String("Host: ") + server + "\r\n");
  client2.print("Host: ");
  client2.println(this->server_sipasi);
  // String useragent = "Arduino-agrieye";
  // client2.print(String("User-Agent: ") + useragent + "\r\n");
  // client2.print("Connection: close\r\n\r\n");
  client2.print("User-Agent: Arduino-agrieye\n");
  client2.println();

  unsigned long timeout = millis();
  while (client2.connected() && millis() - timeout < 20000L) {
    // Print available data (HTTP response from server)
    while (client2.available()) {
      char ccd = client2.read();
      smt1 = String(ccd);
      simpann += ccd;
      
      ccd2 += ccd;
      timeout = millis();
      
    }
  }
  Serial.println(ccd2);

  const char* response = ccd2.c_str();

  const char* bodyStart = strstr(response, "\r\n\r\n");
  if(bodyStart!=NULL){
    bodyStart +=4;
    Serial.println(bodyStart);
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = 
  deserializeJson(doc, bodyStart);
  int valid;
  if(error) {
    Serial.print("Error parsing JSON: ");
    Serial.println(error.c_str());
  } else {
    valid = doc["valid" ];
    Serial.println(valid);
  }
  String version = String(valid);
  
  if(version == "0" || version == "null"){
    Serial.println("Gagal mengirim");
  } else if(version == "1") {
    Serial.println("Berhasil mengirim");
    String tes = "0";
    data2.writeFile(SD,"/configuration/quicksend.txt",tes.c_str());
    delay(3000);
    ESP.restart();
  }
  
}

void sendserver::kirim(String tanggal2, String waktu, float data[], float firmware, uint64_t freeSize){
    this->result = data;
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("tidak berhasil connect kembali");
        String tidakberhasilConnect = "tidak berhasil kembali connect ke Wifi\n";
        data2.appendFile(SD, "/log.txt", tidakberhasilConnect.c_str());
        data2.saveDataLost(tanggal2, waktu, data);
    } else if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" OK");
        String berhasilGPRS = "Berhasil tersambung dengan WIFI\n";
        data2.appendFile(SD, "/log.txt", berhasilGPRS.c_str());
        if (!wifi.connect("www.google.com", 80)) {
          Serial.println("No Internet access available");
          data2.saveDataLost(tanggal2, waktu, data);
        } else {
          Serial.println("internet access available");
          this->state = kirim1(this->result, firmware, freeSize);
          if (this->state == 1) {
            this->state2 = kirim2(this->result, firmware, freeSize);
          } 
          if (this->state2 == 2) {
            kirim3(tanggal2, waktu, data, firmware, freeSize);
          }
        }
    }
  
}

int sendserver::kirim1(float data[], float firmware, uint64_t freeSize){
    String req = "&C0=" + String(data[0]) + "&C1=" + 
                String(data[1]) + "&C2=" + String(data[2]) + "&C3=" + String(data[3]) 
                + "&C4=0.00&C5=" + String(data[4]) + "&C6=" + String(data[5]) + "&C7=" 
                + String(data[6]) + "&C8=" + String(data[7]) + "&C9=" + String(data[8]) 
                + "&C10=" + String(data[9]) + "&C11=" + String(data[10]) + "&C13=" + String(data[13])  + "&C14=" + String(data[11]) + "&C15=" + String(data[12]) + "&X1=" + String(firmware) + "&X2=" + String(freeSize);
    Serial.println(req);
    Serial.println("Performing HTTP POST request...");

    client2.print("GET ");
    // String buatkey = "/feed/api2.php?K=" + String(key) + String(requestData);
    // client2.print(String("GET ") + buatkey + " HTTP/1.0\r\n");
    client2.print("/?key=");
    client2.print(key_sipasi);
    client2.print(req);
    client2.print(" HTTP/1.0\n");
    // client2.print(String("Host: ") + server + "\r\n");
    client2.print("Host: ");
    client2.println(server_sipasi);
    // String useragent = "Arduino-agrieye";
    // client2.print(String("User-Agent: ") + useragent + "\r\n");
    // client2.print("Connection: close\r\n\r\n");
    client2.print("User-Agent: Arduino-agrieye\n");
    client2.println();

    unsigned long timeout = millis();
    while (client2.connected() && millis() - timeout < 20000L) {
      // Print available data (HTTP response from server)
      while (client2.available()) {
        char ccd = client2.read();
        smt1 = String(ccd);
        simpann += ccd;
        
        ccd2 += ccd;
        timeout = millis();
        
      }
    }
    Serial.println(ccd2);

    const char* response = ccd2.c_str();

    const char* bodyStart = strstr(response, "\r\n\r\n");
    if(bodyStart!=NULL){
      bodyStart +=4;
      Serial.println(bodyStart);
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = 
    deserializeJson(doc, bodyStart);
    int valid;
    if(error) {
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
    } else {
      valid = doc["valid" ];
      Serial.println(valid);
    }
    String version = String(valid);
    
    if(version == "0" || version == "null"){
      Serial.println("Gagal mengirim");
      nilaii3 = 1;
    } else if(version == "1") {
      Serial.println("Berhasil mengirim");
      nilaii3 = 2;
    }

    data2.appendFile(SD, "/log.txt", ccd2.c_str());
    if (nilaii3 == 2){
      Serial.println("Data terkirim nih");
      String berhasilTerkirim = "\nBerhasil Terkirim ke Server\n";
      data2.appendFile(SD, "/log.txt", berhasilTerkirim.c_str());
    } 
    else if(nilaii3 == 1){
      Serial.println("Gagal mengirim");
      String gagalmeng = "Gagal mengirim ke server\n";
      data2.appendFile(SD, "/log.txt", gagalmeng.c_str());
      statekirim = 1;
    }
    
    return this->statekirim;

}

int sendserver::kirim2(float data[], float firmware, uint64_t freeSize){
    String req = "&C0=" + String(data[0]) + "&C1=" + 
                String(data[1]) + "&C2=" + String(data[2]) + "&C3=" + String(data[3]) 
                + "&C4=0.00&C5=" + String(data[4]) + "&C6=" + String(data[5]) + "&C7=" 
                + String(data[6]) + "&C8=" + String(data[7]) + "&C9=" + String(data[8]) 
                + "&C10=" + String(data[9]) + "&C11=" + String(data[10]) + "&C13=" + String(data[13])  + "&C14=" + String(data[11]) + "&C15=" + String(data[12]) + "&X1=" + String(firmware) + "&X2=" + String(freeSize);
    Serial.println(req);
    Serial.println("Performing HTTP POST request...");

    client2.print("GET ");
    // String buatkey = "/feed/api2.php?K=" + String(key) + String(requestData);
    // client2.print(String("GET ") + buatkey + " HTTP/1.0\r\n");
    client2.print("/?key=");
    client2.print(key_sipasi);
    client2.print(req);
    client2.print(" HTTP/1.0\n");
    // client2.print(String("Host: ") + server + "\r\n");
    client2.print("Host: ");
    client2.println(server_sipasi);
    // String useragent = "Arduino-agrieye";
    // client2.print(String("User-Agent: ") + useragent + "\r\n");
    // client2.print("Connection: close\r\n\r\n");
    client2.print("User-Agent: Arduino-agrieye\n");
    client2.println();

    unsigned long timeout = millis();
    while (client2.connected() && millis() - timeout < 20000L) {
      // Print available data (HTTP response from server)
      while (client2.available()) {
        char ccd = client2.read();
        smt1 = String(ccd);
        simpann += ccd;
        
        ccd2 += ccd;
        timeout = millis();
        
      }
    }
    Serial.println(ccd2);

    const char* response = ccd2.c_str();

    const char* bodyStart = strstr(response, "\r\n\r\n");
    if(bodyStart!=NULL){
      bodyStart +=4;
      Serial.println(bodyStart);
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = 
    deserializeJson(doc, bodyStart);
    int valid;
    if(error) {
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
    } else {
      valid = doc["valid" ];
      Serial.println(valid);
    }
    String version = String(valid);
    
    if(version == "0" || version == "null"){
      Serial.println("Gagal mengirim");
      nilaii3 = 1;
    } else if(version == "1") {
      Serial.println("Berhasil mengirim");
      nilaii3 = 2;
    }

    data2.appendFile(SD, "/log.txt", ccd2.c_str());
    if (nilaii3 == 2){
      Serial.println("Data terkirim nih");
      String berhasilTerkirim = "\nBerhasil Terkirim ke Server\n";
      data2.appendFile(SD, "/log.txt", berhasilTerkirim.c_str());
    } 
    else if(nilaii3 == 1){
      Serial.println("Gagal mengirim");
      String gagalmeng = "Gagal mengirim ke server\n";
      data2.appendFile(SD, "/log.txt", gagalmeng.c_str());
      statekirim = 1;
    }

    return this->statekirim;

}

void sendserver::kirim3(String tanggal2, String waktu, float data[], float firmware, uint64_t freeSize){
    String req = "&C0=" + String(data[0]) + "&C1=" + 
                String(data[1]) + "&C2=" + String(data[2]) + "&C3=" + String(data[3]) 
                + "&C4=0.00&C5=" + String(data[4]) + "&C6=" + String(data[5]) + "&C7=" 
                + String(data[6]) + "&C8=" + String(data[7]) + "&C9=" + String(data[8]) 
                + "&C10=" + String(data[9]) + "&C11=" + String(data[10]) + "&C13=" + String(data[13])  + "&C14=" + String(data[11]) + "&C15=" + String(data[12]) + "&X1=" + String(firmware) + "&X2=" + String(freeSize);
    Serial.println(req);
    Serial.println("Performing HTTP POST request...");

    client2.print("GET ");
    // String buatkey = "/feed/api2.php?K=" + String(key) + String(requestData);
    // client2.print(String("GET ") + buatkey + " HTTP/1.0\r\n");
    client2.print("/?key=");
    client2.print(key_sipasi);
    client2.print(req);
    client2.print(" HTTP/1.0\n");
    // client2.print(String("Host: ") + server + "\r\n");
    client2.print("Host: ");
    client2.println(server_sipasi);
    // String useragent = "Arduino-agrieye";
    // client2.print(String("User-Agent: ") + useragent + "\r\n");
    // client2.print("Connection: close\r\n\r\n");
    client2.print("User-Agent: Arduino-agrieye\n");
    client2.println();

    unsigned long timeout = millis();
    while (client2.connected() && millis() - timeout < 20000L) {
      // Print available data (HTTP response from server)
      while (client2.available()) {
        char ccd = client2.read();
        smt1 = String(ccd);
        simpann += ccd;
        
        ccd2 += ccd;
        timeout = millis();
        
      }
    }
    Serial.println(ccd2);

    const char* response = ccd2.c_str();

    const char* bodyStart = strstr(response, "\r\n\r\n");
    if(bodyStart!=NULL){
      bodyStart +=4;
      Serial.println(bodyStart);
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = 
    deserializeJson(doc, bodyStart);
    int valid;
    if(error) {
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
    } else {
      valid = doc["valid" ];
      Serial.println(valid);
    }
    String version = String(valid);

    if(version == "0" || version == "null"){
      Serial.println("Gagal mengirim");
      nilaii5 = 1;
    } else if(version == "1") {
      Serial.println("Berhasil mengirim");
      nilaii5 = 2;
    }

    data2.appendFile(SD, "/log.txt", ccd2.c_str());
    
    if(nilaii5 == 1){
        Serial.println("Gagal mengirim");
        String gagalmeng = "Gagal mengirim ke server\n";
        data2.appendFile(SD, "/log.txt", gagalmeng.c_str());
        this->statekirim = 3;
        data2.saveDataLost(tanggal2, waktu, data);
    } else if(nilaii5 = 2) {
        this->statekirim = 0;
        Serial.println("Berhasil terkirim");
        String berhasilTerkirim = "\nBerhasil Terkirim ke Server\n";
        data2.appendFile(SD, "/log.txt", berhasilTerkirim.c_str());
    }
}

void sendserver::kirim_datalost(String buffer)
{
  Serial.println("Performing HTTP POST request...");
  Serial.println(buffer);
  client2.print("GET /feed/api2.php?K=");
  client2.print(this->key_agrieye);
  client2.print(buffer);
  client2.print(" HTTP/1.0\n");
  client2.print("Host: ");
  client2.println(this->server_agrieye);
  client2.print("User-Agent: Arduino-agrieye\n");
  client2.println();
  unsigned long timeout = millis();
  while (client2.connected() && millis() - timeout < 10000L) {
    // Print available data (HTTP response from server)
    while (client2.available()) {
      char cd = client2.read();
      cd2 += cd;
      timeout = millis();
      // appendFile(SD, "/log.txt", cd.c_str());
    }
  }

  Serial.println(cd2);
  if (cd2.indexOf("##1") == -1) {
    nilaii1 = 1;
  }
  else {
    nilaii1 = 2;
  }
  
  if (nilaii1 == 1) {
    this->buffer2 = this->buffer2 + buffer + '\n';
    Serial.println("Data tidak terkirim");
  }
  else if (nilaii1 == 2) {
    Serial.println("Data dari file terkirim nih");
  }
}

String sendserver::getFieldFromJson(String payload, const char* fieldName) {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, payload);
    JsonObject obj = doc.as<JsonObject>();
    
    if (obj.containsKey(fieldName)) {
        return obj[fieldName].as<String>();
    } else {
        return "null";  // Field not found, return an empty string or handle the error
    }
}

t_httpUpdate_return updateFirmware(String url_update)
{
  t_httpUpdate_return ret;

  if(WiFi.status() == WL_CONNECTED)
  {
    ret = ESPhttpUpdate.update(url_update);
    if (ret==HTTP_UPDATE_FAILED){
        Serial.println("[update] update failed.");
    } else if (ret == HTTP_UPDATE_NO_UPDATES) {
        Serial.println("[update] Update no Update");
    } else if (ret == HTTP_UPDATE_OK) {
        Serial.println("[update] Update ok.");
    }
  }

    return ret;

//     switch (ret)
//     {
//       case HTTP_UPDATE_FAILED:
//         Serial.println("[update] update failed.");
//         return ret;
//         break;

//       case HTTP_UPDATE_NO_UPDATES:
//         Serial.println("[update] Update no Update");
//         return ret;
//         break;

//       case HTTP_UPDATE_OK:
//         Serial.println("[update] Update ok.");
//         return ret;
//         break;
//     }
//   }
//   return ret;
}

void sendserver::cekUpdate(String url, String type, double firmware_version)
{
  String response;
  http.begin(url);
  http.GET();
  response = http.getString();
  http.end();
  Serial.println(response);

  String version = getFieldFromJson(response, "version");
  String url_update = getFieldFromJson(response, "url");
  Serial.println(version);
  Serial.println(url_update);

  if(version.toDouble()>firmware_version) {
    Serial.println("Update Available");
    if(updateFirmware(url_update) == HTTP_UPDATE_OK)
    {
      Serial.println("Update Success");
      ESP.restart();
    } else {
      Serial.println("Update Failed");
    }

  } else {
    Serial.println("Update not Available");
  }

}