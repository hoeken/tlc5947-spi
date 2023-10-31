/***************************************************
  This is an example for our Adafruit 24-channel PWM/LED driver
  Pick one up today in the adafruit shop!

  ------> http://www.adafruit.com/products/1429

  These drivers uses SPI to communicate, 3 pins are required to
  interface: Data, Clock and Latch. The boards are chainable

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  Modified to use hardware SPI by Tom Broad (tcbroad@gmail.com)
 ****************************************************/
//
// Created by tombr on 24/07/2019.
// Updated by hoeken on 10/30/2023
//

#ifndef TLC5947_SPI_H
#define TLC5947_SPI_H

#include <Arduino.h>
#include <SPI.h>

#define TLC5947_SPI_MAX_SINGLE 30000000    ///< Max speed with multiple devices
#define TLC5947_SPI_MAX_MULTI  15000000    ///< Max speed with multiple devices
#define TLC5947_SPI_ORDER MSBFIRST         ///< SPI ORDER
#define TLC5947_SPI_MODE SPI_MODE0         ///< SPI MODE

class TLC5947_SPI {
public:
    TLC5947_SPI(uint16_t num = 0, uint8_t cs = SS, SPIClass *theSPI = &SPI);
    ~TLC5947_SPI();

    void begin();
    void setPWM(uint16_t chan, uint16_t pwm);
    uint16_t getPWM(uint16_t chan);
    void setLED(uint16_t lednum, uint16_t r, uint16_t g, uint16_t b);
    void write();
private:
    uint16_t *pwmbuffer;
    uint16_t numdrivers;
    uint32_t speed;

    uint8_t _cs;
    SPIClass *_spi;
};


#endif //TLC5947_SPI_H