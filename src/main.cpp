
// 12v,5v does this matter?
// test with usb-rs485 (where is it?)
// test with 
 
#include <Arduino.h>
#include "SubstrateSensor.h"

CustomModbus myModbus(25, 22, 22, 26);
SubstrateSensor subSensor(&myModbus);

void setup() {
  Serial.begin(9600); // Turn on the "main" serial port for debugging via USB Serial Monitor
  myModbus.begin();
  subSensor.begin();
}
int len = 6;
void loop()
{
  // subSensor.ph(len++);
  // subSensor.moisture();
  Serial.println("--------------------");
  Serial.print("Temparature: ");
  Serial.println(subSensor.read(READ_TEMPERATURE)/10.f);
  Serial.print("Moisture: ");
  Serial.println(subSensor.read(READ_MOISTURE)/10.f);
  Serial.print("Conductivity: ");
  Serial.println(subSensor.read(READ_CONDUCTIVITY));
  Serial.print("Nitrogen: ");
  Serial.println(subSensor.read(READ_NITROGEN));
  Serial.print("Phosphorus: ");
  Serial.println(subSensor.read(READ_PHOSPHORUS));
  Serial.print("Potassium: ");
  Serial.println(subSensor.read(READ_POTASSIUM));
  delay(2000);
}





// works with CAS modbus scanner

/*
byte query[8] = {sensorID, 0x03, 0x07, 0xD0, 0x00, 0x02};
if (modbusSerial.available())
{
  delay(30);
  Serial.println("get: ");
  while (modbusSerial.available() > 0)
    Serial.printf("%02X ", modbusSerial.read());
  Serial.println();

  setTransmit();
  setCRC(query, 8);
  Serial.println("set: ");
  printQuery(query, 8);
  modbusSerial.write(query, 8);
  setReceive();
}
*/




// void loop()
// {
//   uint8_t j, result;
//   uint16_t data[6];
  
//   result = node.readHoldingRegisters(2001, 2);

//   if (result == node.ku8MBSuccess)
//     for (j = 0; j < 6; j++)
//       data[j] = node.getResponseBuffer(j);

//   printQuery(data, 6);

//   delay(2000);
// }

// int count = 0;
// void loop() {
//   // receiving data
//   if(Serial.available()){
//     blinking(300,1);
//     byte input = Serial.read();
//     boolean sendFrame = true;
//     int loops = 8;

//     //getting inquiry Frame blinks twice slow, or blink fast 10x on unusable inputs
//     if(input >= '0' && input <= '9'){
//       prepareInquiryFrame(input-48); 
//       blinking(300,2);
//     } else {
//       blinking(50,10);
//       sendFrame = false;
//     }

//     Serial.println("-----------------------------------");
//     Serial.print("Inquiry Frame Number: ");
//     Serial.println((char)input);    Serial.println();
//     Serial.println("Complete Inquiry Frame:");
//     for(int i = 0; i<8; i++){
//       Serial.print(inquiry_frame[i], HEX);
//       Serial.print(", ");
//     }
//     Serial.println();
//     Serial.print("Calculated CRC: ");
//     Serial.print(inquiry_frame[6], HEX);
//     Serial.print(" ");
//     Serial.println(inquiry_frame[7], HEX);
    
//     setTransmit();
//     if(sendFrame){
//       //Send inquiry frame
//       modbusSerial.write(inquiry_frame, sizeof(inquiry_frame));
//       modbusSerial.flush();
//     } else { 
//       modbusSerial.write(input);
//       modbusSerial.flush();
//     }
//     setReceive();
//   } else if(modbusSerial.available()){
//     char input[100];
//     blinking(100,2);
//     Serial.println();
//     Serial.println("modbusSerial.available(): "+String(modbusSerial.available()));
//     modbusSerial.readBytes(input, 100);
//     Serial.println("strlen(): "+String(strlen(input)));
//     Serial.print("data: ");
//     for (int i = 0; i<strlen(input); i++){
//       Serial.print(input[i], HEX);
//       Serial.print(", ");
//     }
//     Serial.println();
//     Serial.flush();
//     //empty buffer of modbusSerial
//     while(modbusSerial.available()){
//       //delay(100);
//     }
//   } else if (standaloneMode){
//     prepareInquiryFrame(TEMP);
//     //Send inquiry frame
//     modbusSerial.write(inquiry_frame, sizeof(inquiry_frame));
//     modbusSerial.flush();
//     blinking(300, 3);
//     delay(3000);      //blinking 3 times plus delay sums up to 4.8 seconds
//   } else {
//     if(count>20 && !standaloneMode){
//       Serial.print(".");
//       count = 0;
//     } else {
//       count++;
//     }
//     digitalWrite(LED_BUILTIN, LOW);
//   }  
//   Serial.flush();
//   delay(50);
//   count++;
// } 