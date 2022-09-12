#pragma once

#ifndef Pnoible_h
#define Pnoible_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>

#define BLE_SERVER_NAME "LE_Pnoi"
#define BLE_SERVICE_UUID "91bad492-b950-4226-0000-4ede9fa42f59"

#define BLE_LED_CHARACTERISTIC_UUID "cc73b3ba-39f6-4ab3-0000-186dc9577d99"
#define BLE_LED_DESCRIPTOR_UUID BLEUUID((uint16_t)0x2904)

extern bool is_ble_connected;
extern bool is_ble_connecting;

class LEDCharacteristicCallBack : public BLECharacteristicCallbacks
{
public:
    void onWrite(BLECharacteristic *characteristic_) override;
};

class PnoiServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);
};

enum pnoi_action_code
{
    start,
    stop,
    error,
    no_action
};

pnoi_action_code get_action(std::string const &inString);
void init_pnoi_ble();
#endif
