
#ifndef SUBSTRATESENSOR_h
#define SUBSTRATESENSOR_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
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
    SubstrateSensor(CustomModbus *customModbus) : modbus(customModbus) {}
    void begin(uint8_t address=0x01) {addr = address;}

    uint16_t read(read_reg_t);
    float ph(int);
    float ec();
    float temp();
    float moisture();
    float nitrogen();
    float phosphorus();
    float potassium();
};

uint16_t SubstrateSensor::read(read_reg_t read_register)
{
  byte query[8] = {addr, 0x03, 0x00, read_register, 0x00, 0x01};
  byte res[7] = {0};
  modbus->send(query, 8);
  delay(100);
  modbus->get(res, 7);
  return res[4] | res[3] << 8;
}

float SubstrateSensor::temp()
{
  byte query[8] = {addr, 0x03, 0x00, 0x01, 0x00, 0x01};
  byte res[7] = {0};
  modbus->setCRC(query, 8);
  modbus->send(query, 8);
  delay(100);
  modbus->get(res, 7);
  uint16_t val = res[4] | res[3] << 8;
  return val/10.f;
}

float SubstrateSensor::moisture()
{
  byte query[8] = {addr, 0x03, 0x00, 0x00, 0x00, 0x01};
  modbus->setCRC(query, 8);
  Serial.print("query:  ");
  printQuery(query, 8);
  Serial.println();
  modbus->flush();
  modbus->send(query, 8);
  delay(100);

  byte res[7] = {0};
  modbus->readBytes(res, 7);
  Serial.print("res:    ");
  printQuery(res, 7);
  Serial.println();


  byte rescpy[7] = {0};
  memcpy(rescpy, res, 7);
  modbus->setCRC(rescpy, 7);
  Serial.print("rescpy: ");
  printQuery(rescpy, 7);
  Serial.println();

  int moisture = rescpy[4] | rescpy[3] << 8;

  Serial.printf("val: %i\n", moisture);
}

// float SubstrateSensor::ph(int len)
// {
//     byte query[8] = {addr, 0x03, 0x00, 0x03, 0x00, 0x01};
//     // int len = 9;
//     byte res[len];
//     modbus->setCRC(query, 8);
//     modbus->send(query, 8);
//     delay(100);
//     modbus->readBytes(res, len);
//     printQuery(res, len);
//     uint16_t crc = modbus->get_crc(res, len);
//     if (!(res[len-2] == (crc & 0xFF) && res[len-1] == ((crc >> 8) & 0xFF)))
//       Serial.print(" (CRC is not correct)");
//     Serial.println();
// }

#endif