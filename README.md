# Substrate-Sensor-RS485
Arduino/ESP32 library for JXCT's RS485 based substrate NPK sensor (5 pronged version)

Take a look at the example in `src/main.h` to learn how to use it

#### Requirements
- Arduino/ESP32
- UART TTL to RS485 converter. I'm using the MAX485
- JXCT 5 pronged NPK sensor
- The SoftwareSerial library by Peter Lerup and Dirk O. Kaar

## Features
- Can read: pH, TDS, temperature, moisture, conductivity, salinity, nitrogen, phosphorus, potassium
- CRC checking
- write directly to stream

## TODO
- Asynchronous updates for multiple sensors on the same bus
- Changable addresses and baud rates
- Settable calibration values

## Contributors
- Shalom Bermeister (CTRL-ALT-D3L)
