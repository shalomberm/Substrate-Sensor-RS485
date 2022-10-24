#ifndef CUSTOMMODBUS_h
#define CUSTOMMODBUS_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

struct CustomModbus : public SoftwareSerial
{
    uint8_t DI, DE, RE, RO;
    CustomModbus(uint8_t TXpin, uint8_t DEpin, uint8_t REpin, uint8_t RXpin) : DI(TXpin), DE(DEpin), RE(REpin), RO(RXpin) {}
    void begin(uint32_t baud, SoftwareSerialConfig config);
    void send(byte* buffer, size_t size);
    bool get(byte* buffer, size_t size);
    void setCRC(byte* query, size_t size);

private:
    void setReceive();
    void setTransmit();
    uint16_t get_crc(byte* buf, int len);
};

//-------------------------------------------------------------------------------------

void CustomModbus::begin(uint32_t baud=4800, SoftwareSerialConfig config=SWSERIAL_8N1)
{
    pinMode(DE, OUTPUT);
    pinMode(RE, OUTPUT);
    SoftwareSerial::begin(baud, config, RO, DI, false, 256, 0);
    setReceive();
}

void CustomModbus::send(byte* buffer, size_t size)
{
    SoftwareSerial::flush();
    setTransmit();
    setCRC(buffer, size);
    SoftwareSerial::write(buffer, size);
    setReceive();
}

bool CustomModbus::get(byte* buffer, size_t size)
{
    Serial.println(SoftwareSerial::readBytes(buffer, size));
    // Serial.printf("0x%02X\n", buffer[size-1]);
    int crc = get_crc(buffer, size-2);
    return (buffer[size-2] == (crc & 0xFF) 
           && buffer[size-1] == ((crc >> 8) & 0xFF));
    // if (this->available())
    // {
    //     // Serial.println("get: ");
    //     delay(50);
    //     int i = 0;
    //     while (this->available() > 0)
    //         buffer[i++] = this->read();
    //         // Serial.printf("%02X ", read());
    //     // Serial.println();
    // }
}

void CustomModbus::setCRC(byte *query, size_t size)
{
    int crc = get_crc(query, size-2);
    //extracting low byte and high byte
    query[size-2] = (crc & 0xFF);
    query[size-1] = ((crc >> 8) & 0xFF);
}

void CustomModbus::setReceive()
{
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
}

void CustomModbus::setTransmit()
{
    digitalWrite(DE, HIGH);
    digitalWrite(RE, HIGH);
}

uint16_t CustomModbus::get_crc(byte* buf, int len)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos];      // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--)    // Loop over each bit
        {
        if ((crc & 0x0001) != 0)      // If the LSB is set
        {
            crc >>= 1;                  // Shift right and XOR 0xA001
            crc ^= 0xA001;
        }
        else crc >>= 1; // Else LSB is not set just shift right
        }
    }
    return crc; // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
}

//-------------------------------------------------------------------------------------

void printQuery(uint8_t *query, int len)
{
  for (int i = 0; i < len; i++)
    Serial.printf("%02X ", query[i]);
  // Serial.println();
}
void printQuery(uint16_t *query, int len)
{
  for (int i = 0; i < len; i++)
    Serial.printf("%04X ", query[i]);
  // Serial.println();
}

#endif