
#ifndef SUBSTRATESENSOR_h
#define SUBSTRATESENSOR_h

#include <Arduino.h>
#include "CustomModbus.h"

enum read_reg_t : uint8_t
{
    READ_MOISTURE = 0,
    READ_TEMPERATURE,
    READ_CONDUCTIVITY,
    READ_PH,
    READ_NITROGEN,
    READ_PHOSPHORUS,
    READ_POTASSIUM,
    READ_SALINITY,
    READ_TDS
};

class SubstrateSensor
{
public:
    uint8_t addr;
    CustomModbus *modbus;
    uint16_t data[9] = {0};

    SubstrateSensor(CustomModbus *customModbus) : modbus(customModbus) {}
    void begin(uint8_t address=0x01) {addr = address;}
    void update();
    uint16_t read(read_reg_t);

    float ph() {return data[READ_PH]/10.f;}
    float tds() {return data[READ_TDS];}
    float temp() {return data[READ_TEMPERATURE]/10.f;}
    float moisture() {return data[READ_MOISTURE]/10.f;}
    float conductivity() {return data[READ_CONDUCTIVITY];}
    float salinity() {return data[READ_SALINITY];}
    float nitrogen() {return data[READ_NITROGEN];}
    float phosphorus() {return data[READ_PHOSPHORUS];}
    float potassium() {return data[READ_POTASSIUM];}
};

void SubstrateSensor::update()
{
  // addr (1), func (1), start addr (2), length (2), crc (2),
  // addr (1), func (1), valid bytes (1), nth data (2*n), crc (2),
  byte query[8] = {addr, 0x03, 0x00, 0x00, 0x00, 0x09};
  byte res[23] = {0};
  modbus->send(query, 8);
  modbus->get(res, 23);

  // check CRC before moving on

  for (int i = 0; i < 9; i++)
    data[i] = res[i*2+4] | res[i*2+3] << 8;
}

uint16_t SubstrateSensor::read(read_reg_t read_register)
{
  byte query[8] = {addr, 0x03, 0x00, read_register, 0x00, 0x01};
  byte res[7] = {0};
  modbus->send(query, 8);
  modbus->get(res, 7);
  // Serial.printf("(%i) ", );
  return res[4] | res[3] << 8;
}

#endif