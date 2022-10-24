
// 12v,5v does this matter?
 
#include <Arduino.h>
#include "SubstrateSensor.h"

#define TXpin 17
#define RXpin 16
#define SWITCH 12

CustomModbus myModbus(TXpin, SWITCH, SWITCH, RXpin);
SubstrateSensor subSensor(&myModbus);

void printSubSensor()
{
  subSensor.update();
  Serial.println("--------------------");
  Serial.printf("Address: 0x%02X\n", subSensor.addr);
  Serial.printf("Baud rate: %i\n", subSensor.modbus->baudRate());
  Serial.print("Temparature: ");
  Serial.println(subSensor.temp());
  Serial.print("Moisture: ");
  Serial.println(subSensor.moisture());
  Serial.print("PH: ");
  Serial.println(subSensor.ph());
  Serial.print("TDS: ");
  Serial.println(subSensor.tds());
  Serial.print("Conductivity: ");
  Serial.println(subSensor.conductivity());
  Serial.print("Salinity: ");
  Serial.println(subSensor.salinity());
  Serial.print("Nitrogen: ");
  Serial.println(subSensor.nitrogen());
  Serial.print("Phosphorus: "); 
  Serial.println(subSensor.phosphorus());
  Serial.print("Potassium: ");
  Serial.println(subSensor.potassium());
}

void setup() {
  Serial.begin(9600); // Main serial port for debugging via USB Serial Monitor
  myModbus.begin(4800); // Modbus serial port for substrate sensor communication
  subSensor.begin(0xC);
}

int i = 0;
void loop()
{
  printSubSensor();
  delay(2000);

  // if (i++ > 5)
  //   subSensor.setAddress(0x69);
  // if (i > 10)
  //   subSensor.setBaudRate(NPK_BAUD_RATE_9600);
}
