# ESP32_Process_Big_Data_Process

This Arduino sketch for ESP32 is designed for 16MB ESP32 boards(Can also work with 4MB/8MB ESP with default partition scheme, the storage of 27% will not be used), utilizing the default partition scheme of 3MB for APP and 9.9MB for FATFS. The code facilitates the retrieval and processing of fingerprint data stored on a server. The data file it downloads is 3MB, containing information for 750 IDs. This version is an updated iteration of a previous project, where data was manually uploaded to FATFS using tools in the Arduino IDE.

## Features

- WiFi connection setup.
- Data download from URLs.(GitHub file raw data)
- FFat file system usage.
- Parsing and processing of downloaded fingerprint data.
- User input for finding specific IDs.
- Serial communication for debugging.

## Getting Started

1. **WiFi Configuration:**
   - Replace `SSID` and `PASSWORD` with your WiFi credentials.

2. **URLs for Fingerprint Data:**
   - Modify the `FileUrl` array with the URLs containing fingerprint data.

3. **ESP32 Configuration:**
   - Ensure that you are using a 16MB ESP32 and the default partition scheme of 3MB APP / 9.9MB FATFS

4. **Upload to ESP32:**
   - Use the Arduino IDE or your preferred method to upload the sketch to your ESP32.

5. **Serial Communication:**
   - Open the Serial Monitor to view debugging information and interact with the code.

## Data File Details

- **File Size:** 3MB
- **Number of IDs:** 750

## Previous Project

- [Link to Previous Project](https://github.com/my-dudhwala/ESP_Big_Data)

## Dependencies

- WiFi library
- WiFiMulti library
- HTTPClient library
- FS (File System) library
- FFat library

## Usage

1. Input an ID through the Serial Monitor.
2. The code retrieves and prints two fingerprint data entries associated with that ID.

## Author

[Mohammed Yasar Dudhwala]
