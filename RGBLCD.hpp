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

#ifndef NHD_RGBLCD_HPP
#define NHD_RGBLCD_HPP
// Component
#include "LCD.hpp"

// Arduino
#include <Arduino.h>

// Standard
#include <stdint.h>

namespace nhd
{
/// @brief LCD base class
/// @tparam R number of rows
/// @tparam C number of cols
/// @todo make this virtual
template<unsigned int R, unsigned int C>
class RGBLCD : public LCD<R,C>
{
 public:
  /// @brief constructor for the RGBLCD interface
  /// @param lcd_addr address for the LCD panel to be controlled.
  ///                 Default from manual: http://www.newhavendisplay.com/specs/NHD-0216K3Z-FSRGB-FBW-V3.pdf
  RGBLCD(uint8_t red, uint8_t green, uint8_t blue, uint8_t lcd_addr = 0x50) :
    LCD<R,C>(lcd_addr),
    m_red_pin{red},
    m_green_pin{green},
    m_blue_pin{blue}
  {
  }

  /// @brief init the 
  void init()
  {
    LCD<R,C>::init();
    pinMode(m_red_pin, OUTPUT);
    pinMode(m_green_pin, OUTPUT);
    pinMode(m_blue_pin, OUTPUT);
  }
  
  void setColor(uint8_t r, uint8_t g, uint8_t b)
  {
    analogWrite(m_red_pin, r);
    analogWrite(m_green_pin, g);
    analogWrite(m_blue_pin, b);
  }

 private:
  uint8_t m_red_pin;
  uint8_t m_green_pin;
  uint8_t m_blue_pin;
};
} // namespace nhd

#endif //NHD_LCD_HPP
