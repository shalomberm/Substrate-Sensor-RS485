
#ifndef SUBSTRATESENSOR_h
#define SUBSTRATESENSOR_h

//https://khunsomsak.medium.com/7-in-1-soil-sensor-e7063e6646fb

#include <Arduino.h>
#include "CustomModbus.h"

enum npk_register_t : uint16_t
{
    READ_MOISTURE =     0x00,
    READ_TEMPERATURE =  0x01,
    READ_CONDUCTIVITY = 0x02,
    READ_PH =           0x03,
    READ_NITROGEN =     0x04,
    READ_PHOSPHORUS =   0x05,
    READ_POTASSIUM =    0x06,
    READ_SALINITY =     0x07,
    READ_TDS =          0x08,
    WRITE_ADDR =        0x7D0,
    WRITE_BAUD =        0x7D1
};

enum npk_baud_rate_t : uint8_t
{
  NPK_BAUD_RATE_2400 = 0x00,
  NPK_BAUD_RATE_4800 = 0x01,
  NPK_BAUD_RATE_9600 = 0x02
};

class SubstrateSensor
{
public:
    uint8_t addr;
    CustomModbus *modbus;
    uint16_t data[9] = {0};
    bool valid = false;

    SubstrateSensor(CustomModbus *customModbus, uint8_t address) : modbus(customModbus), addr(address) {}
    void update();
    uint16_t read(npk_register_t);

    float ph() {return isValid() ? data[READ_PH]/10.f : NAN;}
    float tds() {return isValid() ? data[READ_TDS] : NAN;}
    float temp() {return isValid() ? data[READ_TEMPERATURE]/10.f : NAN;}
    float moisture() {return isValid() ? data[READ_MOISTURE]/10.f : NAN;}
    float conductivity() {return isValid() ? data[READ_CONDUCTIVITY] : NAN;}
    float salinity() {return isValid() ? data[READ_SALINITY] : NAN;}
    float nitrogen() {return isValid() ? data[READ_NITROGEN] : NAN;}
    float phosphorus() {return isValid() ? data[READ_PHOSPHORUS] : NAN;}
    float potassium() {return isValid() ? data[READ_POTASSIUM] : NAN;}
    bool isValid() {return valid;}
    bool setAddress(uint8_t address); // these dont work yet
    bool setBaudRate(npk_baud_rate_t baud_rate); // these dont work yet
    void writeTo(Stream &stream);
};

void SubstrateSensor::update()
{
  memset(data, 0, 9); // clear list
  valid = false; // default is false
  // addr (1), func (1), start addr (2), length (2), crc (2),
  // addr (1), func (1), valid bytes (1), nth data (2*n), crc (2),
  byte query[8] = {addr, 0x03, 0x00, 0x00, 0x00, 0x09};
  byte res[23] = {0};
  modbus->send(query, 8);
  if (modbus->get(res, 23) == 0)
    return;
  for (int i = 0; i < 9; i++)
    data[i] = res[i*2+4] | res[i*2+3] << 8;
  valid = true;
  // Serial.printf("update %i\n", this->addr);
  // Serial.printf("done %i\n", this->addr);
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

void SubstrateSensor::writeTo(Stream &stream)
{
  stream.printf("Address: 0x%02X\n", this->addr);
  stream.printf("Baud rate: %i\n", this->modbus->baudRate());
  stream.print("Temparature: ");
  stream.println(this->temp());
  stream.print("Moisture: ");
  stream.println(this->moisture());
  stream.print("PH: ");
  stream.println(this->ph());
  stream.print("TDS: ");
  stream.println(this->tds());
  stream.print("Conductivity: ");
  stream.println(this->conductivity());
  stream.print("Salinity: ");
  stream.println(this->salinity());
  stream.print("Nitrogen: ");
  stream.println(this->nitrogen());
  stream.print("Phosphorus: "); 
  stream.println(this->phosphorus());
  stream.print("Potassium: ");
  stream.println(this->potassium());
}

#endif