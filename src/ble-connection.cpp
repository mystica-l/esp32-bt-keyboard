#include "ble-connection.h"

// Defined variables
// UUID for the Human Interface Device (HID) Service (my keyboard)
const char* hidServiceUUID = "00001812-0000-1000-8000-00805f9b34fb";
// const char* hidReportCharUUID = "00002a4d-0000-1000-8000-00805f9b34fb";

BLEScan* pBLEScan = nullptr; 
BLEClient* pClient = BLEDevice::createClient(); 
BLERemoteCharacteristic* pHidReportChar = nullptr; 
bool isConnected = false; 

class MyNotificationCallback : public BLECharacteristicCallbacks
{
  void onNotify(BLECharacteristic* pCharacteristic) override
  {
    Serial.println("Received HID report:");

    std::string value = pCharacteristic->getValue(); // Get the data as a string
    size_t length = value.length();
    const uint8_t* data = (const uint8_t*)value.data(); // Convert string to byte array

    for (size_t i = 0; i < length; i++) {
      Serial.printf("0x%02X ", data[i]); // Print each byte in hexadecimal
    }
    Serial.println();

    // Decode HID keycodes (starting from the 3rd byte)
    if (length > 2) { 
      for (size_t i = 2; i < length; i++) { 
        if (data[i] != 0) { 
          Serial.printf("Key pressed: 0x%02X\n", data[i]);
        }
      }
    }
  }

  void onWrite(BLECharacteristic* pCharacteristic, esp_ble_gatts_cb_param_t* param) override
  {
    Serial.println("Received HID write report:");
  }
};

/* initialize */
void initialize()
{
  Serial.begin(115200);
  Serial.println("Starting BLE HID scan...");

  // Initialize BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); // Get a BLE scanner instance
  pBLEScan->setActiveScan(true);   // Enable active scanning
}

void scan() {
  // Check if the connection is still active and set the flag
  if (!pClient->isConnected())
  {
    Serial.println("Disconnected from HID device.");
    isConnected = false;
  }
  else
  {
    Serial.println("Already connected to the HID device.");
    isConnected = true;
  }

  if(!isConnected)
  {
    Serial.println("Scanning for BLE devices...");
    BLEScanResults scanResults = (pBLEScan->start(5)); // Scan for 5 seconds

    for (int i = 0; i < scanResults.getCount(); i++)
    {
      BLEAdvertisedDevice device = scanResults.getDevice(i);
      // Check if the device advertises the HID service UUID
      if (device.haveServiceUUID() && device.isAdvertisingService(BLEUUID(hidServiceUUID)))
      {
        Serial.println("HID device found!");
        Serial.printf("Device address: %s\n", device.getAddress().toString().c_str());

        // Stop scanning and connect to the device
        pBLEScan->stop();
        connectToDevice(device.getAddress());
        
        // Break out of the loop
        return;
      }
    }
  }

  if(isConnected)
  {
    delay(500);
  }
}

void connectToDevice(BLEAddress address)
{
  Serial.println("Connecting to HID device...");
  pClient = BLEDevice::createClient();

  if (pClient->connect(address))
  {
    Serial.println("Connected to HID device!");
    isConnected = true;
    /*
    // Get the HID service
    BLERemoteService* pHidService = pClient->getService(hidServiceUUID);
    if (pHidService)
    {
      // Get the HID report characteristic
      pHidReportChar = pHidService->getCharacteristic(hidReportCharUUID);
      if (pHidReportChar && pHidReportChar->canNotify())
      {
        Serial.println("Subscribing to HID report notifications...");
        pHidReportChar->registerForNotify(new MyNotificationCallback());
      }
      else
      {
        Serial.println("HID Report characteristic not found or notifications not supported.");
      }
    }
    else
    {
      Serial.println("HID service not found");
    }
    */
    // print all information about the BLE client
    Serial.println(pClient->toString().c_str());
  }
  else
  {
    Serial.println("Failed to connect to the HID device.");
    isConnected = false;
  }
}