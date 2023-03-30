/*
 Copyright (C) 2012 Andy Karpov <andy.karpov@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __VS1003_H__
#define __VS1003_H__

// STL headers
// C headers
// Framework headers
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


/**
 * Driver for VS1003 - MP3 / WMA / MIDI Audio Codec Chip
 *
 * See http://www.vlsi.fi/en/products/vs1003.html
 */

class VS1003
{
private:
  uint8_t cs_pin; /**< Pin where CS line is connected */
  uint8_t dcs_pin; /**< Pin where DCS line is connected */
  uint8_t dreq_pin; /**< Pin where DREQ line is connected */
  uint8_t reset_pin; /**< Pin where RESET line is connected */
  uint8_t my_SPCR; /**< Value of the SPCR register how we like it. */
  uint8_t my_SPSR; /**< Value of the SPSR register how we like it. */
protected:
  inline void await_data_request(void) const
  {
    while ( !digitalRead(dreq_pin) );
  }

  inline void control_mode_on(void) const
  {
    digitalWrite(dcs_pin,HIGH);
    digitalWrite(cs_pin,LOW);
  }

  inline void control_mode_off(void) const
  {
    digitalWrite(cs_pin,HIGH);
  }

  inline void data_mode_on(void) const
  {
    digitalWrite(cs_pin,HIGH);
    digitalWrite(dcs_pin,LOW);
  }

  inline void data_mode_off(void) const
  {
    digitalWrite(dcs_pin,HIGH);
  }
  inline void save_our_spi(void)
  {
    my_SPSR = SPSR;
    my_SPCR = SPCR;
  }
  inline void set_our_spi(void)
  {
    SPSR = my_SPSR;
    SPCR = my_SPCR;
  }
  uint16_t read_register(uint8_t _reg) const;
  void write_register(uint8_t _reg,uint16_t _value) const;
  void sdi_send_buffer(const uint8_t* data,size_t len);
  void sdi_send_zeroes(size_t length);
  void print_byte_register(uint8_t reg) const;

  /**
   * Load a user code plugin
   *
   * @param buf Location of memory (in PROGMEM) where the code is
   * @param len Number of words to load
   */
  void loadUserCode(const uint16_t* buf, size_t len) const;

  void loadMidiCode() const;

public:

  /**
   * Constructor
   *
   * Only sets pin values.  Doesn't do touch the chip.  Be sure to call begin()!
   */
  VS1003( uint8_t _cs_pin, uint8_t _dcs_pin, uint8_t _dreq_pin, uint8_t _reset_pin);

  /**
   * Begin operation
   *
   * Sets pins correctly, and prepares SPI bus.
   */
  void begin(void);

  /**
   * Initialize real time MIDI
   *
  */
  void beginMidi(void);

  /**
   * Send a MIDI message
  */
  void sendMidiMessage(uint8_t cmd, uint8_t data1, uint8_t data2);

  /**
   * Prepare to start playing
   *
   * Call this each time a new song starts.
   */
  void startSong(void);

  /**
   * Play a chunk of data.  Copies the data to the chip.  Blocks until complete.
   *
   * @param data Pointer to where the data lives
   * @param len How many bytes of data to play
   */
  void playChunk(const uint8_t* data, size_t len);

  /**
   * Finish playing a song.
   *
   * Call this after the last playChunk call.
   */
  void stopSong(void);

  /**
   * Print configuration details
   *
   * Dumps all registers to stdout.  Be sure to have stdout configured first
   * (see fdevopen() in avr/io.h).
   */
  void printDetails(void) const;

  /**
   * Set the player volume
   *
   * @param vol Volume level from 0-255, lower is louder.
   */
  void setVolume(uint8_t vol) const;
};

#endif // __VS1003_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
