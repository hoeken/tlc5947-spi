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
// Modified by hoeken on 10/30/2023
//

#include "TLC5947_SPI.h"
#include <SPI.h>

TLC5947_SPI::TLC5947_SPI(uint16_t num, uint8_t cs, SPIClass *theSPI) {
    numdrivers = num;
    _cs = cs;
    _spi = theSPI;

    if (numdrivers == 1)
        speed = TLC5947_SPI_MAX_SINGLE;
    else
        speed = TLC5947_SPI_MAX_MULTI;

    this->pwmbuffer = (uint16_t *) malloc(2 * 24 * num);
    memset(this->pwmbuffer, 0, 2 * 24 * num);
}

TLC5947_SPI::~TLC5947_SPI() { free(pwmbuffer); }

void TLC5947_SPI::begin() {

    //pinMode(MOSI, OUTPUT);
    //pinMode(SCK, OUTPUT);

    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    _spi->begin();

    //SPI.setBitOrder(MSBFIRST);
    //SPI.setDataMode(SPI_MODE0);
    //SPI.begin();
}

void TLC5947_SPI::setPWM(uint16_t chan, uint16_t pwm) {
    if (pwm > 4095) {
        pwm = 4095;
    }

    if (chan > 24 * this->numdrivers) {
        return;
    }

    this->pwmbuffer[chan] = pwm;
}

uint16_t TLC5947_SPI::getPWM(uint16_t chan) {
  if (chan >= 24 * numdrivers) {
    return 0;
  } else {
    return pwmbuffer[chan];
  }
}

void TLC5947_SPI::setLED(uint16_t lednum, uint16_t r, uint16_t g, uint16_t b) {
    this->setPWM(lednum * 3, r);
    this->setPWM(lednum * 3 + 1, g);
    this->setPWM(lednum * 3 + 2, b);
}

void TLC5947_SPI::write() {
    _spi->beginTransaction(SPISettings(speed, TLC5947_SPI_ORDER, TLC5947_SPI_MODE));
    digitalWrite(_cs, LOW);

    // 24 channels per TLC5974
    for (int16_t c = 24 * numdrivers - 1; c >= 0; c -= 2) {
        // 12 bits per channel, send MSB first
        // have to split a pair of channels into groups of 3 bytes

        _spi->transfer((pwmbuffer[c] >> 4) & 0x00FF);
        _spi->transfer(((pwmbuffer[c] << 4) & 0x00F0) | ((pwmbuffer[c - 1] >> 8) & 0x000F));
        _spi->transfer(pwmbuffer[c - 1] & 0x00FF);
    }

    digitalWrite(_cs, HIGH);
    _spi->endTransaction();
}
