// EEPROM 24C04 library for Arduino, 512 x 8 (4K), v0.2
// 30.06.2017, Arduino IDE v1.8.3, LZ2WSG
//---------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include "at24c04.h"
//       _____
//  NC -|  ^  |- VCC
//  A1 -|     |- WP(GND)
//  A2 -|     |- SCL
// GND -|_____|- SDA

// A1(GND), A2(GND) -> I2C = 0x50 or 0x51
//---------------------------------------------------------------------------------------------------------
at24c04::at24c04(uint8_t _i2c_addr) {
  m_chip_addr = _i2c_addr;                                   // I2C адрес на EEPROM
}
//---------------------------------------------------------------------------------------------------------
void at24c04::init() {                                       // инициализация на библиотеката
  Wire.begin();
}
//---------------------------------------------------------------------------------------------------------
void at24c04::write(uint16_t m_addr, uint8_t m_data) {       // записва клетка на адрес m_addr с данни m_data
  if (m_addr < 512) {
    uint8_t p0 = (uint8_t)(m_addr >> 8);
    Wire.beginTransmission(m_chip_addr | p0);
    Wire.write((uint8_t)m_addr);
    Wire.write(m_data);
    Wire.endTransmission();
    delay(5);
  }
}
//---------------------------------------------------------------------------------------------------------
void at24c04::update(uint16_t m_addr, uint8_t m_data) {      // чете клетка и записва данни само при разлика
  if (m_addr < 512) {                                        // eeprom endurance
    uint8_t m_exist_data = read(m_addr);
    if (m_data != m_exist_data) {                            // записвай ако данните за запис са различни от записаните
      uint8_t p0 = (uint8_t)(m_addr >> 8);
      Wire.beginTransmission(m_chip_addr | p0);
      Wire.write((uint8_t)m_addr);
      Wire.write(m_data);
      Wire.endTransmission();
      delay(5);
    }
  }
}
//---------------------------------------------------------------------------------------------------------
uint8_t at24c04::read(uint16_t m_addr) {                     // чете съдържанието на клетка с адрес m_addr
  uint8_t p0 = (uint8_t)(m_addr >> 8);
  uint8_t m_data = 0xFF;

  Wire.beginTransmission(m_chip_addr | p0);
  Wire.write((uint8_t)m_addr);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)(m_chip_addr | p0), (uint8_t)1);
  if (Wire.available())
    m_data = Wire.read();
  return m_data;
}
