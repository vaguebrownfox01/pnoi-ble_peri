#include "pnoi_ble.h"

// LED Characteristic and Descriptor
BLECharacteristic pnoiLEDCharacteristics(BLE_LED_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor pnoiLEDDescriptor(BLE_LED_DESCRIPTOR_UUID);

bool is_ble_connected = false;

class CharacteristicCallBack : public BLECharacteristicCallbacks
{
public:
    // This method not called
    void onWrite(BLECharacteristic *characteristic_) override
    {

        std::string d = characteristic_->getValue();
        size_t dl = characteristic_->getLength();

        Serial.println(d.c_str());
    }
};

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        is_ble_connected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        is_ble_connected = false;
    }
};

void init_pnoi_ble()
{
    // Create the BLE Device
    BLEDevice::init(BLE_SERVER_NAME);

    // Create BLE server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create BLE Service
    BLEService *primaryService = pServer->createService(BLE_SERVICE_UUID);

    // Create BLE Characteristics
    primaryService->addCharacteristic(&pnoiLEDCharacteristics);
    pnoiLEDDescriptor.setValue("Pnoi LED");
    pnoiLEDCharacteristics.addDescriptor(&pnoiLEDDescriptor);
    pnoiLEDCharacteristics.setCallbacks(new CharacteristicCallBack);

    primaryService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}