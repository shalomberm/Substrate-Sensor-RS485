
#ifndef SUBSTRATESENSOR_h
#define SUBSTRATESENSOR_h

//https://khunsomsak.medium.com/7-in-1-soil-sensor-e7063e6646fb

#include <Arduino.h>
#include "CustomModbus.h"

enum npk_register_t : uint16_t
{
    READ_MOISTURE = 0,
    READ_TEMPERATURE,
    READ_CONDUCTIVITY,
    READ_PH,
    READ_NITROGEN,
    READ_PHOSPHORUS,
    READ_POTASSIUM,
    READ_SALINITY,
    READ_TDS,
    WRITE_ADDR = 0x7D0,
    WRITE_BAUD = 0x7D1
};

enum npk_baud_rate_t : uint8_t
{
  NPK_BAUD_RATE_2400 = 0,
  NPK_BAUD_RATE_4800,
  NPK_BAUD_RATE_9600
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
    uint16_t read(npk_register_t);

    float ph() {return data[READ_PH]/10.f;}
    float tds() {return data[READ_TDS];}
    float temp() {return data[READ_TEMPERATURE]/10.f;}
    float moisture() {return data[READ_MOISTURE]/10.f;}
    float conductivity() {return data[READ_CONDUCTIVITY];}
    float salinity() {return data[READ_SALINITY];}
    float nitrogen() {return data[READ_NITROGEN];}
    float phosphorus() {return data[READ_PHOSPHORUS];}
    float potassium() {return data[READ_POTASSIUM];}
    bool setAddress(uint8_t address); // these dont work yet
    bool setBaudRate(npk_baud_rate_t baud_rate); // these dont work yet
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

uint16_t SubstrateSensor::read(npk_register_t read_register)
{
  byte query[8] = {addr, 0x03, 0x00, (uint8_t)read_register, 0x00, 0x01};
  byte res[7] = {0};
  modbus->send(query, 8);
  modbus->get(res, 7);
  // Serial.printf("(%i) ", );
  return res[4] | res[3] << 8;
}

bool SubstrateSensor::setAddress(uint8_t address)
{
  if (address < 255)
  {
    byte query[8] = {addr, 0x03, (WRITE_ADDR>>8)&0xff, (WRITE_ADDR>>0)&0xff, 0x00, address};
    byte res[7] = {0};
    modbus->send(query, 8);
    modbus->get(res, 7);
    addr = address;
    return true;
  }
  return false;
}

bool SubstrateSensor::setBaudRate(npk_baud_rate_t baud_rate)
{
  byte query[8] = {addr, 0x03, (WRITE_BAUD>>8)&0xff, (WRITE_BAUD>>0)&0xff, 0x00, baud_rate};
  byte res[7] = {0};
  modbus->send(query, 8);
  modbus->get(res, 7);
  int getBaud[] = {2400, 4800, 9600};
  modbus->begin(getBaud[baud_rate]);
  return true;
}

#endif