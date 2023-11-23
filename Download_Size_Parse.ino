/*
  ESP32 Fingerprint Data Retrieval

  This Arduino sketch for ESP32 establishes a WiFi connection, downloads data from
  predefined URLs, and stores it in the FFat file system. The code allows users to
  input an ID, and it retrieves and processes fingerprint data associated with that ID.

  Features:
  - WiFi connection setup.
  - 2-3 MB Data download from URLs.
  - FFat file system usage.
  - Parsing and processing of downloaded fingerprint data.
  - User input for finding specific IDs.
  - Serial communication for debugging.

  Dependencies:
  - WiFi, WiFiMulti, HTTPClient, FS, FFat libraries.

  Key Details:
  - Retrieves and prints two fingerprint data entries for each user-input ID.

  Author: Mohammed Yasar Dudhwala
  Date: 22/11/2023
*/


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "FS.h"
#include "FFat.h"

String MainString = "";
String ParsedString = "";
int userEmployeeID;
int EndCount = 0;
String TempString;

String path = "/Data750.txt";

// You only need to format FFat the first time you run a test
#define FORMAT_FFAT true

#define USE_SERIAL Serial
WiFiMulti wifiMulti;

#define SSID "Magician's Web"
#define PASSWORD "1234ABCD"

String FileUrl[] = {
  "https://raw.githubusercontent.com/my-dudhwala/ESP_Big_Data/main/Sample_data/data1to375.txt",
  "https://raw.githubusercontent.com/my-dudhwala/ESP_Big_Data/main/Sample_data/data376To750.txt",
  "https://raw.githubusercontent.com/my-dudhwala/ESP_Big_Data/main/Sample_data/data750.txt"
};

void printDirectory(File dir, int numTabs = 3);
void setup() {

  USE_SERIAL.begin(500000);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  if (FORMAT_FFAT) FFat.format();
  if (!FFat.begin()) {
    Serial.println("FFat Mount Failed");
    return;
  }

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  wifiMulti.addAP(SSID, PASSWORD);

  Download();// Download the data
  Count();// Count the received IDs

  unsigned int totalBytes = FFat.totalBytes();
  unsigned int usedBytes = FFat.usedBytes();
  unsigned int freeBytes  = FFat.freeBytes();

  Serial.println("File sistem info.");

  Serial.print("Total space:      ");
  Serial.print(totalBytes);
  Serial.println("byte");

  Serial.print("Total space used: ");
  Serial.print(usedBytes);
  Serial.println("byte");

  Serial.print("Total space free: ");
  Serial.print(freeBytes);
  Serial.println("byte");

  Serial.println();

  // Open dir folder
  File dir = FFat.open("/");
  // Cycle all the content
  printDirectory(dir);
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("\n\nReading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void loop() {
  //  Download();
  //  readFile(FFat, "/Data750.txt");
  if (Serial.available() > 0) {
    userEmployeeID = Serial.parseInt();
    FindString(userEmployeeID);
  }
}

void Count() {

  Serial.println("\nCounting total recieved IDs...");

  if (!FFat.begin(true)) {
    Serial.println("An Error has occurred while mounting FFat");
    return;
  }

  File file = FFat.open(F("/data750.txt"), "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  String NumString;
  int NullIndex;
  int NumInt;
  char inChar;

  while (file.available()) {
    inChar = file.read();
    MainString += inChar;
    if (inChar == '"') {
      inChar = file.read();
      MainString += inChar;
      if (isDigit(inChar)) {
        NumString += inChar;
        inChar = file.read();
        MainString += inChar;
        if (isDigit(inChar)) {
          NumString += inChar;
          inChar = file.read();
          MainString += inChar;
          if (isDigit(inChar)) {
            NumString += inChar;
            inChar = file.read();
            MainString += inChar;
          }
        }
        NumInt = NumString.toInt();
        if (inChar == '"') {
          Serial.print(NumInt);
          Serial.print(", ");
          //          Serial.println(".");
        }
      }
    }
    MainString = "";
    NumString = "";
  }
}

void FindString(int RequiredID) {
  Serial.println("\nFinding entered ID.....");

  if (!FFat.begin(true)) {
    Serial.println("An Error has occurred while mounting FFat");
    return;
  }

  File file = FFat.open(F("/data750.txt"), "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  String NumString;
  int NullIndex;
  int NumInt;
  char inChar;

  while (file.available()) {
    inChar = file.read();
    MainString += inChar;
    if (inChar == '"') {
      inChar = file.read();
      MainString += inChar;
      if (isDigit(inChar)) {
        NumString += inChar;
        inChar = file.read();
        MainString += inChar;
        if (isDigit(inChar)) {
          NumString += inChar;
          inChar = file.read();
          MainString += inChar;
          if (isDigit(inChar)) {
            NumString += inChar;
            inChar = file.read();
            MainString += inChar;
          }
        }
        NumInt = NumString.toInt();
        if (inChar == '"') {
          Serial.print(NumInt);
          Serial.print(", ");
          if (NumInt == RequiredID) {
            Serial.print("\nFound ID number ");
            Serial.print(" : ");
            Serial.println(NumInt);
            NumString = "";
            while (file.available()) {
              inChar = file.read();
              MainString += inChar;
              NullIndex = MainString.indexOf("NULL");
              if (inChar == '}') {
                processString(MainString);
                MainString = "";
                return;
              }

              if (NullIndex > 0) {
                Serial.println("");
                Serial.println("NULL");
                NullIndex = 0;
                MainString = "";
                return;
              }
            }
          }
        }
      }
    }
    MainString = "";
    NumString = "";
  }
  file.close();
  FFat.end();
}

void processString(String NextString) {
  Serial.println("Processing the string...");
  Serial.println("");
  int firstQuote = NextString.indexOf('"');
  int secondQuote = NextString.indexOf('"', firstQuote + 1);

  String ParsedStringNum = "";
  ParsedStringNum = NextString.substring(firstQuote + 1, secondQuote);
  int DataNum =  ParsedStringNum.toInt();

  int FirstColon = NextString.indexOf(':');
  int Comma = NextString.indexOf(',', NextString.indexOf(',') + 1);

  String FirstFinger = NextString.substring(FirstColon + 4, Comma - 2);
  Serial.print("First finger : ");
  Serial.println(FirstFinger);

  int SecondColon = NextString.indexOf(':', NextString.indexOf(':') + 1 );
  int EndMark = NextString.lastIndexOf('n');

  String SecondFinger = NextString.substring(SecondColon + 4, EndMark + 1);
  Serial.print("Second finger : ");
  Serial.println(SecondFinger);
  Serial.println("");
}

void Download() {

  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    //    for (int i = 0; i <= 1; i++) {
    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    USE_SERIAL.println(FileUrl[2]);
    // configure server and url
    http.begin(FileUrl[2]);

    //http.begin("192.168.1.12", 80, "/test.html");

    //USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    File file = FFat.open("/Data750.txt", "w");
    if (!file) {
      Serial.println("- failed to open file for writing");
      return;
    }
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        //        File file = FFat.open(path, FILE_WRITE);
        //        if (!file) {
        //          Serial.println("- failed to open file for writing");
        //          return;
        //        }
        // get length of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();

        // create buffer for read
        uint8_t buff[128] = { 0 };

        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();

          if (size) {
            // read up to 128 byte
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            USE_SERIAL.write(buff, c);

            if (file.write(buff, c)) {
              //              Serial.println("- file written");
            } else {
              Serial.println("- write failed");
              return;
            }

            if (len > 0) {
              len -= c;
            }
          }
          delay(1);
        }

        USE_SERIAL.println();
        USE_SERIAL.print("[HTTP] connection closed or file end.\n");

      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    file.close();
    //    }
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
