////////////////////////////////////////////////////////////////////////
// NHD LCD Driver
// Copyright (C) 2020 VAAS
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Initial Author: Anton Pozharskiy (apozharski@gmail.com)
// Date Added: 19/06/2020
////////////////////////////////////////////////////////////////////////

#ifndef NHD_LCD_HPP
#define NHD_LCD_HPP

#include <Arduino.h>
#include <Wire.h>

// @todo (apozharski) move this to a submodule
// Types
#include "types/BasicOnOff.hpp"

// Standard
#include <stdint.h>

namespace nhd
{
/// @brief LCD base class
/// @tparam R number of rows
/// @tparam C number of cols
/// @todo make this virtual
template<unsigned int R, unsigned int C>
class LCD
{
 public:
  /// @brief constructor for the LCD interface
  /// @param lcd_addr address for the LCD panel to be controlled.
  ///                 Default from manual: http://www.newhavendisplay.com/specs/NHD-0216K3Z-FSRGB-FBW-V3.pdf
  LCD(uint8_t lcd_addr = 0x50) :
    m_lcd_addr{lcd_addr}
  {
  }

  /// @brief initializes the LCD panel, clearing everything off of it.
  /// @note  This assumes `Wire.begin()` has been called in order to initialize the twi interface.
  void init()
  {
    // Set the twi to normal speed.
    Wire.setClock(10000);
    /// @todo (apozharskiy) what else do I need to do here? Probably nothing but :shrug:
  }

  uint8_t setLcdState(types::BasicOnOff state)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    switch(state)
    {
     case types::BasicOnOff::ON:
       Wire.write(0x41);
       break;
     case types::BasicOnOff::OFF:
       Wire.write(0x42);
       break;
    }
    return Wire.endTransmission();
  }

  uint8_t homeCursor()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x46);
    return Wire.endTransmission();
  }

  uint8_t turnOnUnderline()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x47);
    return Wire.endTransmission();
  }

  uint8_t cursorRight()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x4A);
    return Wire.endTransmission();
  }

  uint8_t cursorLeft()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x49);
    return Wire.endTransmission();
  }
  
  uint8_t setCursorPosition(uint8_t r, uint8_t c)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x45);
    /// @todo do check bounds here and more generalized the position calculation. 
    uint8_t pos = r*0x4 + c;
    Wire.write(pos);
    return Wire.endTransmission();
  }

  uint8_t clearScreen()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x51);
    return Wire.endTransmission();
  }

  uint8_t setDisplayContrast(uint8_t contrast)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x52);
    /// @todo do contrast bound's check.
    Wire.write(contrast);
    return Wire.endTransmission();
  }

  uint8_t setBacklightBrightness(uint8_t brightness)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x53);
    /// @todo do brightness bounds check
    Wire.write(brightness);
    return Wire.endTransmission();
  }
  uint8_t sendChar(char c)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(c);
    return Wire.endTransmission();
  }

  LCD& operator<<(char* s)
  {
    while(*s != 0)
    {
      Wire.beginTransmission(m_lcd_addr);
      Wire.write(*s);
      Wire.endTransmission();
      cursorRight();
      s++;
    }
    return *this;
  }
 private:
  uint8_t m_lcd_addr;
  char m_screen_buffer[R][C]; /// @todo make << operate with the screen_buffer. 
};
} // namespace nhd

#endif //NHD_LCD_HPP
