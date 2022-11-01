
// 12v,5v does this matter?
 
#include <Arduino.h>
#include "SubstrateSensor.h"

#define TXpin 17
#define RXpin 16
#define SWITCH 32

CustomModbus myModbus(TXpin, SWITCH, SWITCH, RXpin);
SubstrateSensor subSensor(&myModbus, 0xC);

void setup() {
  Serial.begin(9600); // Main serial port for debugging via USB Serial Monitor
  myModbus.begin(4800); // Modbus serial port for substrate sensor communication
}

int i = 0;
void loop()
{
  subSensor.update();
  subSensor.writeTo(Serial);
  delay(2000);

  // if (i++ > 5)
  //   subSensor.setAddress(0x69);
  // if (i > 10)
  //   subSensor.setBaudRate(NPK_BAUD_RATE_9600);
}
