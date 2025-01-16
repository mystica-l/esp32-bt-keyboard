#ifndef BLE_CONNECTION_H
#define BLE_CONNECTION_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <Arduino.h>

extern const char* hidServiceUUID;
extern BLEScan* pBLEScan;
extern BLEClient* pClient;
extern bool isConnected; // Flag to track connection status

void initialize();
void scan();
void connectToDevice(BLEAddress);

#endif