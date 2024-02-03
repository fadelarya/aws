#ifndef READANDFILTER_H
#define READANDFILTER_H

#include <Arduino.h>
#include "modbus/ModbusMaster232.h"
#include <Adafruit_INA219.h>

class akuisisi : public ModbusMaster232{
    public:
        akuisisi(int _SSRPin);
        float get_temp();
        float get_humidity();
        float get_atm_pressure();
        float get_wind_speed();
        float get_wind_dir();
        float get_rain();
        float get_rainHL();
        float get_rain0();
        float get_rain1();
        float get_rainBasic();
        float get_radiance();
        float get_pm25();
        float get_pm10();
        float get_voltage();
        float get_power();
        float * get_all();
        void print_all(float data[]);
        String print(float data[]);
        void setupSensor();
        void setupBegin();
        void turnOnSensor();
        void turnOffSensor();
        int SSRPin;

};

#endif