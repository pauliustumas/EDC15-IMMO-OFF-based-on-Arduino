#include <Wire.h>
// Code is taken from https://www.kn34pc.com/spr/arduino_24c04.html 
// Запис, четене и изтриване на клетки в EEPROM 24C04, 512 x 8 (4K)
// 30.06.2017, Arduino IDE v1.8.3, LZ2WSG
//---------------------------------------------------------------------------------------------------------
#include "at24c04.h"
//       _____
//  NC -|  ^  |- VCC
//  A1 -|     |- WP(GND)
//  A2 -|     |- SCL
// GND -|_____|- SDA

at24c04 eeprom(0x50);                             // A1(GND), A2(GND) -> I2C = 0x50 или 0x51

char ch;
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(57600);

  eeprom.init();
  // Wire.setClock(400000L);
}
//---------------------------------------------------------------------------------------------------------
void loop() {
  Serial.println("EEPROM 24C04:");
  Serial.println("r - Read");
  Serial.println("x - immo off");
  Serial.println();

  while (1) {
    if (Serial.available() > 0) {
      ch = Serial.read();

      if (ch == 'r')                              // read
        eeprom_read();

      if (ch == 'x') {                            // write random data and read
        eeprom_write_random_data();
        eeprom_read();
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------------
void eeprom_read() {
  Serial.print("         ");
  for (uint8_t i = 0; i < 16; i++) {
    Serial.print('0');
    Serial.print(i, HEX);-
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t j = 0; j < 32; j++) {
    if (j < 16)
      Serial.print('0');
    Serial.print("00000");
    Serial.print(j, HEX);
    Serial.print("0 ");
    for (uint8_t k = 0; k < 16; k++) {
      int add = j * 16 + k;
      uint8_t read_byte = eeprom.read(add);
      if (read_byte < 16)
        Serial.print('0');
        
        
      if((add == 432 || add == 478) && read_byte == 0x33){
        Serial.print("XX");
      }
      else{
        Serial.print(read_byte, HEX);
      }
        
      
      Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();
}

//---------------------------------------------------------------------------------------------------------
void eeprom_write_random_data() {
    eeprom.update(432, 0x60);
    eeprom.update(478, 0x60);
}
