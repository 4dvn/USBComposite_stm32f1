// This uses the greiman sdfat library.
#include <CompositeSerial.h>
#include <MassStorage.h>
#include <SPI.h>
#include "SdFat.h"

#define LED_PIN PB12

SdFat sd;
const uint32_t speed = SPI_CLOCK_DIV2;
const uint8_t SD_CHIP_SELECT = SS;
bool enabled = false;
uint32 cardSize;

bool write(uint32_t memoryOffset, const uint8_t *writebuff, uint16_t transferLength) {
  return sd.card()->writeBlocks(memoryOffset/512, writebuff, transferLength/512);
}

bool read(uint32_t memoryOffset, uint8_t *readbuff, uint16_t transferLength) {
  return sd.card()->readBlocks(memoryOffset/512, readbuff, transferLength/512);
}

void setup() {
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,1);
}

void initReader() {
  digitalWrite(LED_PIN,0);
  cardSize = sd.card()->cardSize();  
  USBMassStorage.setDrive(0, cardSize, 512, read, write);
  USBComposite.add(USBMassStorage);
  USBComposite.add(CompositeSerial);
  USBComposite.begin();
  enabled=true;
}

void loop() {
  if (!enabled) {
    if (sd.cardBegin(SD_CHIP_SELECT, speed)) {
      initReader();
    }
    else {
      delay(50);
    }
  }
  else {
    USBMassStorage.loop();
  }
}

