#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include <U8x8lib.h>

int screenDataPin = 2;
int screenClockPin = 3;

int sdChipSelectPin = 4;

U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(screenClockPin, screenDataPin);

void setup() {
  //setup screen
  u8x8.begin();
  u8x8.clear();
  u8x8.setCursor(0,0);
  u8x8.setFont(u8x8_font_chroma48medium8_r); 

  u8x8.print("Screen Ready.");
  u8x8.setCursor(0,1);

  if (!SD.begin(4)) {
    u8x8.print("SD Init Fail!");
    while (1);
  } else {
    u8x8.print("SD Card Ready.");
  }

  u8x8.setCursor(0,2);
}

void loop() {
  
}
