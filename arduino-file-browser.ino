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

void setup() {
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
  setFileCounts(root, dirCount, fileCount);

  String countLabel = "File Count: ";
  countLabel.concat(fileCount);
  u8x8.print(countLabel);

  firstFileNumber = 1; 
  lastFileNumber = 4;
  
  delay(500);

  refreshMenu();
}

void loop() {
  
}

void refreshMenu() {
  int count = 0;
  
  u8x8.clear();
  
  for(int i = firstFileNumber; i <= lastFileNumber; i++) {
    u8x8.setCursor(0,count);
    u8x8.print(getFileName(root, i));
    count++;
  }
}

void setFileCounts(File dir, int & dirCount, int & fileCount) {
  dirCount = 0;
  fileCount = 0;
  
  while(true) {
    File entry = dir.openNextFile();
    if(!entry) {
      dir.rewindDirectory();
      break;
    } else {
      if(!entry.isDirectory()) {
        fileCount++;
      } else {
        dirCount++;
      }
    }
  }
}

char * getFileName(File dir, int fileNumber) {
  int fileCount = 0;
  
  while(true) {
    File entry = dir.openNextFile();
    
    if(!entry) {
      dir.rewindDirectory();
    } else {
      if(!entry.isDirectory()){
        fileCount++;
  
        if(fileCount == fileNumber) {
          dir.rewindDirectory();
          return entry.name();
        }
      }
    }
  }
}
