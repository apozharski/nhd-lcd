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
#include <cstdint>

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
  LCD(std::uint8_t lcd_addr = 0x50) :
    m_lcd_addr{addr}
  {
  }

  /// @brief initializes the LCD panel, clearing everything off of it.
  /// @note  This assumes `Wire.init()` has been called in order to initialize the twi interface.
  void LCD::init()
  {
    // Set the twi to normal speed.
    Wire.setClock(100000);
    /// @todo (apozharskiy) what else do I need to do here? Probably nothing but :shrug:
  }

  void LCD::setLcdState(types::BasicOnOff state)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    switch(state)
    {
     case types::BasicOnOff::ON:
       Wire.write(0x41);
     case types::BasicOnOff::OFF:
       Wire.write(0x42);
    }
    Wire.endTransmission();
  }

  void LCD::homeCursor()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x46);
    Wire.endTransmission();
  }

  void LCD::cursorRight()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x4A);
    Wire.endTransmission();
  }

  void LCD::cursorLeft()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x49);
    Wire.endTransmission();
  }
  
  void LCD::setCursorPosition(std::uint8_t r, std::uint8_t c)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x45);
    /// @todo do check bounds here and more generalized the position calculation. 
    std::uint8_t pos = r*0x4 + c;
    Wire.write(pos);
    Wire.endTransmission();
  }

  void LCD::clearScreen()
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x51);
    Wire.endTransmission();
  }

  void LCD::setDisplayContrast(std::uint8_t contrast)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x52);
    /// @todo do contrast bound's check.
    Wire.write(contrast);
    Wire.endTransmission();
  }

  void LCD::setBacklightBrightness(std::uint8_t brightness)
  {
    Wire.beginTransmission(m_lcd_addr);
    Wire.write(0xFE);
    Wire.write(0x53);
    /// @todo do brightness bounds check
    Wire.write(brightness);
    Wire.endTransmission();
  }

  LCD& operator<<(std::string)
  {
    /// @todo implement this
    return *this;
  }
 private:
  std::uint8_t m_lcd_addr;
  char m_screen_buffer[R][C];
};
} // namespace nhd

#endif //NHD_LCD_HPP