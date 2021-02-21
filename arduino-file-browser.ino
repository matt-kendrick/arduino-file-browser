#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include <U8x8lib.h>

int screenDataPin = 2;
int screenClockPin = 3;

int sdChipSelectPin = 4;

U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(screenClockPin, screenDataPin);

File root;

int fileCount, dirCount, firstFileNumber, lastFileNumber;

int testButtonPin = 10;

boolean goingDown = true;

String fileNames[16] = {};

void setup() {
  pinMode(testButtonPin, INPUT_PULLUP);
  
  //setup screen
  u8x8.begin();
  u8x8.clear();
  u8x8.setCursor(0,0);
  u8x8.setFont(u8x8_font_chroma48medium8_r); 
  u8x8.print("Screen Ready.");
  u8x8.setCursor(0,1);

  if (!SD.begin(sdChipSelectPin)) {
    u8x8.print("SD Init Fail!");
    while (1);
  }

  u8x8.print("SD Card Ready.");
  
  u8x8.setCursor(0,2);
  root = SD.open("/");
  setFileCounts(root);

  String countLabel = "File Count: ";
  countLabel.concat(fileCount);
  u8x8.print(countLabel);

  firstFileNumber = 0; 
  lastFileNumber = 3;

  refreshMenu();
}

void loop() {
  if (digitalRead(testButtonPin) == LOW) {
     if(goingDown) {
      firstFileNumber++;
      lastFileNumber++;
     }

     if(!goingDown) {
      firstFileNumber--;
      lastFileNumber--;
     }

     if(lastFileNumber == fileCount) {
      goingDown = false;
     }
     
     if(firstFileNumber == 0) {
      goingDown = true;
     }

     u8x8.clear();
     u8x8.setCursor(15,1);
     u8x8.print(firstFileNumber);
     u8x8.setCursor(15,2);
     u8x8.print(lastFileNumber);
     
     refreshMenu();
  }
}

void refreshMenu() {
  int count = 0;
  
  u8x8.clear();
  
  for(int i = firstFileNumber; i <= lastFileNumber; i++) {
    u8x8.setCursor(0,count);
    u8x8.print(fileNames[i]);
    count++;
  }
}

void setFileCounts(File dir) {
  dirCount = 0;
  fileCount = 0;
  
  while(true) {
    File entry = dir.openNextFile();
    if(!entry) {
      dir.rewindDirectory();
      break;
    } else {
      if(!entry.isDirectory()) {
        fileNames[fileCount] = entry.name();
        fileCount++;
      } else {
        dirCount++;
      }
    }
  }
}
