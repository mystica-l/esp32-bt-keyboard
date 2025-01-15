#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <Arduino.h>

// UUID for the Human Interface Device (HID) Service (my keyboard)
const char* hidServiceUUID = "00001812-0000-1000-8000-00805f9b34fb";
BLEScan* pBLEScan;
BLEClient* pClient = BLEDevice::createClient();
bool isConnected = false; // Flag to track connection status

void initialize();
void scan();
void connectToDevice(BLEAddress);