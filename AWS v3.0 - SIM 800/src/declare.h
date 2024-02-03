#define WDT_TIMEOUT 250

//-----Deklarasi SSID & AP Access Point-----//
const char *SSID_AP = "AWS Kedung Putri";
const char *PWD_AP = "12345678";

// const char *SSID = "CPE200-4G4E7C60";
// const char *PWD = "ptcp!2023";

const char *SSID = "Device Kedungputri";
const char *PWD = "ptcp!2023";


// --Apikey kedungputri--//
String keyyy = "awsarip";
String key = "f48e2ef3be4a0672bfd1b24d7838e8c4";

//--SERVER KEDUNGPUTRI--//
const char* server = "api.smartagri.id";
const char* server2 = "api.smartagri.id";

const char* server3 = "agrieye.tp.ugm.ac.id";

//--PIN SSR KEDUNGPUTRI--//
int SSRPin = 27;

//--PIN-BUTTON-KEDUNGPUTRI--//
int buttonPin = 35;

// //--Apikey Sapon--//
// String keyyy = "SmartAWSSapon";
// String key = "c4b5917781471f5fb0125c8400acdb6a";

// //--SERVER SAPON--//
// const char* server = "api.sipasi.online";
// const char* server2 = "api.sipasi.online";

// const char* server3 = "agrieye.tp.ugm.ac.id";

// // --PIN SSR SAPON--//
// int SSRPin = 13;

// //--PIN-BUTTON-SAPON
// int buttonPin = 4;

int ledPin = 2;


int pengkali = 1000000;

bool maintenanceState;
float * result;
uint64_t cardSize, usedSize, freeSize;

String tanggal, tanggal2, waktu, password, nama;
int rssiPercentage, durasisleep;
float a_temp,a_humidity,a_atm_press,a_wind_speed,a_wind_dir,a_rainfall,a_radiance,a_pm25,a_pm10,a_voltage,a_power;
float c_temp,c_humidity,c_atm_press,c_wind_speed,c_wind_dir,c_rainfall,c_radiance,c_pm25,c_pm10,c_voltage,c_power;