#include <SD.h>
#include <SPI.h>
#include <Keyboard.h>

#include <U8x8lib.h>

/*
 * Pin 2 - OLED Data (SDA)
 * Pin 3 - OLED Clock (SCK)
 * Pin 18 - SD Chip Select (CS)
 * Pin 15 - SD (CLK)
 * Pin 14 - SD (MISO)
 * Pin 16 - SD (MOSI)
 */

const int screenDataPin = 2;
const int screenClockPin = 3;
const int sdChipSelectPin = 18;
const int testButtonPin = 10;
const int linePerPage = 4;

U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(screenClockPin, screenDataPin);

File root;
int fileCount, firstOption, selectedLine;
String fileNames[16] = {};

void setup() {
  Serial.begin(9600);

  Keyboard.begin();
  
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

  refreshMenu();
  
  u8x8.setCursor(0, selectedLine);
  u8x8.print(">" + cleanFileName(fileNames[firstOption + selectedLine]));
}

void loop() {
  if (digitalRead(testButtonPin) == LOW) {
     moveSelector();
     delay(500);
  }
}

void typeFileContents() {
  String fileName = fileNames[firstOption + selectedLine];

  File myFile;
  
  myFile = SD.open(fileName);
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Keyboard.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + fileName);
  }
}

String cleanFileName(String fileName) {
  fileName.substring(0,15);
  fileName.replace(".TXT","");
  return fileName;
}

void moveSelector() {
  u8x8.setCursor(0,selectedLine);
  u8x8.print(" " + cleanFileName(fileNames[firstOption + selectedLine]));
  selectedLine++;
  
  if(selectedLine == linePerPage) {
    selectedLine = 0;

    if(firstOption + linePerPage >= fileCount) {
      firstOption = 0;
    } else {
      firstOption += linePerPage;
    }

    refreshMenu();
  }

  u8x8.setCursor(0,selectedLine);
  u8x8.print(">" + cleanFileName(fileNames[firstOption + selectedLine]));
  typeFileContents();
}

void refreshMenu() {
  int count = 0;
  
  u8x8.clear();
  
  for(int i = firstOption; i <= firstOption + linePerPage; i++) {
    u8x8.setCursor(0,count);
    u8x8.print(" " + cleanFileName(fileNames[i]));
    count++;
  }
}

void setFileCounts(File dir) {
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
      }
    }
  }
}
