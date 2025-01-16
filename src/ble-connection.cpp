#include "ble-connection.h"

// Defined variables
// UUID for the Human Interface Device (HID) Service (my keyboard)
const char* hidServiceUUID = "00001812-0000-1000-8000-00805f9b34fb";
BLEScan* pBLEScan = nullptr; 
BLEClient* pClient = BLEDevice::createClient(); 
bool isConnected = false; 

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
  delay(100);
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
    Serial.println("Stable connection.");
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
  }
  else
  {
    Serial.println("Failed to connect to the HID device.");
    isConnected = false;
  }
}