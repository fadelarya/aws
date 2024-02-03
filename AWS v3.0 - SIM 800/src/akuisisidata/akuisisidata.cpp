#include "akuisisidata.h"

ModbusMaster232 aws1(102, Serial2); // Temperature, Humidity, Pressure
ModbusMaster232 aws2(200, Serial2); // Wind Speed & Wind Direction
ModbusMaster232 aws3(202, Serial2); // Rainfall
ModbusMaster232 aws4(204, Serial2); // Radiation
ModbusMaster232 aws5(138, Serial2); // PM 2.5 & PM 10

Adafruit_INA219 ina219;

akuisisi::akuisisi(int _SSRPin){
    this->SSRPin = _SSRPin;
}

float akuisisi::get_temp()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {

        aws1.readHoldingRegisters(0x0, 1);
        result = aws1.getResponseBuffer(0)/100.00; //Satuan Celcius
        aws1.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_humidity()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {
        aws1.readHoldingRegisters(0x1, 1);
        result = aws1.getResponseBuffer(0)/100.00;  //Satuan %
        aws1.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_atm_pressure()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {
        aws1.readHoldingRegisters(0x2, 1);
        result = aws1.getResponseBuffer(0)/10.00;  //Satuan hPa
        aws1.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_wind_speed()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {
        aws2.readHoldingRegisters(0x0, 1);
        result = aws2.getResponseBuffer(0) * 0.036;  //Satuan Km/h
        // result = aws2.getResponseBuffer(0)/100.00; //Satuan m/s
        aws2.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_wind_dir()
{
    float result = 0.00;
    aws2.readHoldingRegisters(0x1, 1);
    result = aws2.getResponseBuffer(0)/1.00;  //Satuan derajat
    aws2.clearResponseBuffer();
    
    return result;
}

float akuisisi::get_rain()
{
    float result = 0.00;
    aws3.readHoldingRegisters(0x1, 1);
    result = aws3.getResponseBuffer(0)/10.00;  //Satuan mm/5 menit
    aws3.clearResponseBuffer();
    
    return result;
}

float akuisisi::get_rainHL()
{
    int result[2];
    float result2 = 0.00;
    float hasil = 0.00;
    aws3.readHoldingRegisters(0x0,2);
    result[0] = aws3.getResponseBuffer(0);
    result[1] = aws3.getResponseBuffer(1);
    aws3.clearResponseBuffer();
    
    result2 = (result[0] * 256) + result[1];
    hasil = result2 / 50.00;
    return hasil;
}

float akuisisi::get_rain0()
{
    float result = 0.00;
    aws3.readHoldingRegisters(0x0, 1);
    result = aws3.getResponseBuffer(0);  //Satuan mm/5 menit
    aws3.clearResponseBuffer();
    
    return result;
}

float akuisisi::get_rain1()
{
    float result = 0.00;
    aws3.readHoldingRegisters(0x1, 1);
    result = aws3.getResponseBuffer(0);  //Satuan mm/5 menit
    aws3.clearResponseBuffer();

    aws3.writeSingleRegister(0x6,1);
    
    return result;
}

float akuisisi::get_rainBasic()
{
    float result = 0.00;
    aws3.readHoldingRegisters(0x1, 1);
    result = aws3.getResponseBuffer(0)/50.00;  //Satuan mm/5 menit
    aws3.clearResponseBuffer();

    aws3.writeSingleRegister(0x6,1);
    
    return result;
}


float akuisisi::get_radiance()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {
        aws4.readHoldingRegisters(0x5, 1);
        result = aws4.getResponseBuffer(0)/1.00;  //Satuan W/m^2
        aws4.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_pm25()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {
        aws5.readHoldingRegisters(0x1, 1);
        result = aws5.getResponseBuffer(0)/1.00;  //PM 2.5
        aws5.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_pm10()
{
    float result = 0.00;
    float result2 = 0.00;
    for(int a=0;a<9;a++)
    {
        aws5.readHoldingRegisters(0x2, 1);
        result = aws5.getResponseBuffer(0)/1.00;  //PM 10
        aws5.clearResponseBuffer();
        result2 += result;
        delay(100);
    }

    result = result2/9.00;
    
    return result;
}

float akuisisi::get_voltage(){
    float result = ina219.getBusVoltage_V();
    return result;
}

float akuisisi::get_power(){
    float result = ina219.getPower_mW();
    return result;
}

float * akuisisi::get_all()
{
    static float result[14];
    result[0] = get_voltage();
    result[1] = get_radiance();
    delay(500);
    result[2] = get_atm_pressure();
    result[3] = get_power();
    delay(500);
    result[4] = get_temp();
    delay(500);
    result[5] = get_humidity();
    delay(500);

    float rain0 = get_rain0();
    float rain1 = get_rain1();

    result[6] = rain1 / 10.00;
    result[11] = ((rain0 * 256.00) + rain1) / 50.00;
    result[12] = rain1 / 50.00;
    result[13] = rain1 / 5.00;
        
    // result[6] = get_rain();
    // delay(1000);
    // result[11] = get_rainHL();
    // delay(1000);
    // result[12] = get_rainBasic();
    
    char formattedFloat[10]; // Adjust the array size as needed
    snprintf(formattedFloat, sizeof(formattedFloat), "%.1f", result[6]);
    // Print the formatted float
    Serial.println(formattedFloat);

    // char formattedFloat2[10];
    // snprintf(formattedFloat2, sizeof(formattedFloat2), "%.1f", result[11]);
    // // Print the formatted float
    // Serial.println(formattedFloat2);
    
    // char formattedFloat3[10];
    // snprintf(formattedFloat3, sizeof(formattedFloat3), "%.1f", result[12]);
    // // Print the formatted float
    // Serial.println(formattedFloat3);

    // Convert the formattedFloat back to a float
    result[6]= atof(formattedFloat);
    // result[11]= atof(formattedFloat2);
    // result[12]= atof(formattedFloat3);

    result[7] = get_wind_speed();
    result[8] = get_wind_dir();
    result[9] = get_pm25();
    result[10] = get_pm10();

    return result;
}

void akuisisi::print_all(float data[])
{
    Serial.print("temp       : ");Serial.println(data[4]);
    Serial.print("humidity   : ");Serial.println(data[5]);
    Serial.print("atm pres   : ");Serial.println(data[2]);
    Serial.print("wind speed : ");Serial.println(data[7]);
    Serial.print("wind dir   : ");Serial.println(data[8]);
    Serial.print("rainfall   : ");Serial.println(data[6]);
    Serial.print("radiance   : ");Serial.println(data[1]);
    Serial.print("pm 2.5     : ");Serial.println(data[9]);
    Serial.print("pm 10      : ");Serial.println(data[10]);
    Serial.print("voltage    : ");Serial.println(data[0]);
    Serial.print("power      : ");Serial.println(data[3]);
    Serial.println("+++++++++++++++++++++");
}

String akuisisi::print(float data[])
{
  String nilaisave = "Suhu = " + String(data[4]) + ", Humidit = " + String(data[5]) + ", Atm Pressure = " + String(data[2]) + ", Wind Speed = " + String(data[7]) + ", Wind Direction = " + String(data[8]) + ", Rainfall = " + String(data[6]) + ", Radiance = " + String(data[1]) + ", pm 2.5 = " + String(data[9]) + ", pm 10 = " + String(data[10]) + ", Voltage = " + String(data[0]) + ", Power = " + String(data[3]) + "\n";
  return nilaisave;
}

void akuisisi::setupSensor(){
    Serial2.begin(9600);
    pinMode(this->SSRPin, OUTPUT);
}

void akuisisi::setupBegin(){
    aws1.begin();
    aws2.begin();
    aws3.begin();
    aws4.begin();
    aws5.begin();
    ina219.begin();
}

void akuisisi::turnOnSensor(){
    digitalWrite(this->SSRPin, HIGH);
}

void akuisisi::turnOffSensor(){
    digitalWrite(this->SSRPin, LOW);
}


