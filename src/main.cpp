
// 12v,5v does this matter?
 
#include <Arduino.h>
#include "SubstrateSensor.h"

CustomModbus myModbus(25, 22, 22, 26);
SubstrateSensor subSensor(&myModbus);

void setup() {
  Serial.begin(9600); // Main serial port for debugging via USB Serial Monitor
  myModbus.begin(4800); // Modbus serial port for substrate sensor communication
  subSensor.begin(0x01);
}

void loop()
{
  subSensor.update();
  Serial.println("--------------------");
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
  delay(2000);
}
