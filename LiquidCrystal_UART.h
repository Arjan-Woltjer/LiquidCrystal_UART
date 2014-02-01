/*
LiquidCrystal_UART.h - based on SoftwareSerial included in Arduino IDE 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef LiquidCrystal_UART_h
#define LiquidCrystal_UART_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

// Defines for lcd screen
#define COMMAND 0xFE
#define CLEAR   0x01
#define SPCOMM  0x7C
#define BACKLIGHT 0x8F

class LiquidCrystal_UART : public Stream
{
private:
  // per object data
  uint8_t _receivePin;
  uint8_t _receiveBitMask;
  volatile uint8_t *_receivePortRegister;
  uint8_t _transmitBitMask;
  volatile uint8_t *_transmitPortRegister;

  uint16_t _rx_delay_centering;
  uint16_t _rx_delay_intrabit;
  uint16_t _rx_delay_stopbit;
  uint16_t _tx_delay;

  uint16_t _buffer_overflow:1;
  uint16_t _inverse_logic:1;

  // static data
  static char _receive_buffer[_SS_MAX_RX_BUFF]; 
  static volatile uint8_t _receive_buffer_tail;
  static volatile uint8_t _receive_buffer_head;
  static LiquidCrystal_UART *active_object;

  // private methods
  void recv();
  uint8_t rx_pin_read();
  void tx_pin_write(uint8_t pin_state);
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);

  // private static method for timing
  static inline void tunedDelay(uint16_t delay);
  
  // added functionality by MeijWorks
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _numlines;
  
  uint8_t _screencounter;
  uint8_t _cursor_row;
  uint8_t _cursor_col;
  char _buffer [4][21];
  char _screen [4][21];

public:
  // public methods
  LiquidCrystal_UART(uint8_t receivePin, uint8_t transmitPin, uint8_t lcd_cols, uint8_t lcd_rows, bool inverse_logic = false);
  ~LiquidCrystal_UART();
  void begin(long speed);
  bool listen();
  void end();
  bool isListening() { return this == active_object; }
  bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
  int peek();

  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();
  
  using Print::write;

  // public only for easy access by interrupt handlers
  static inline void handle_interrupt();
  
  // added functionality by MeijWorks
  void init();
  void backlight();
  void clear();
  void setCursor(uint8_t col, uint8_t row);
  void write_screen(uint8_t n);
  void write_buffer(char * line, uint8_t line_no);
  void write_buffer(char character, uint8_t line_no, uint8_t col_no);
};

// Arduino 0012 workaround
//#undef int
//#undef char
//#undef long
//#undef byte
//#undef float
//#undef abs
//#undef round

#endif
