#include "pnoi_ble.h"
#include "pnoi_ext.h"

// LED Characteristic and Descriptor
BLECharacteristic pnoiLEDCharacteristics(BLE_LED_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor pnoiLEDDescriptor(BLE_LED_DESCRIPTOR_UUID);

bool is_ble_connected = false;
bool is_ble_connecting = false;

void LEDCharacteristicCallBack::onWrite(BLECharacteristic *charx_)
{
    const char *action = charx_->getValue().c_str();

    switch (get_action(action))
    {
    case start:
        glow_green();
        break;

    case stop:
        glow_blue();
        break;

    case error:
        glow_red();
        break;

    default:
        glow_red();
        break;
    }

    Serial.println(action);
}

void PnoiServerCallbacks::onConnect(BLEServer *pServer)
{
    is_ble_connected = true;
    is_ble_connecting = false;
    glow_blue();
}

void PnoiServerCallbacks::onDisconnect(BLEServer *pServer)
{
    is_ble_connected = false;
    is_ble_connecting = true;
}

pnoi_action_code get_action(std::string const &inString)
{
    if (inString == "start")
        return start;
    if (inString == "stop")
        return stop;
    if (inString == "error")
        return error;

    return no_action;
};

void init_pnoi_ble()
{
    // Create the BLE Device
    BLEDevice::init(BLE_SERVER_NAME);

    // Create BLE server
    BLEServer *ble_server = BLEDevice::createServer();
    ble_server->setCallbacks(new PnoiServerCallbacks());

    // Create BLE Service
    BLEService *ble_service = ble_server->createService(BLE_SERVICE_UUID);

    // Create BLE Characteristics
    ble_service->addCharacteristic(&pnoiLEDCharacteristics);
    pnoiLEDDescriptor.setValue("Pnoi LED INDICATOR");
    pnoiLEDCharacteristics.addDescriptor(&pnoiLEDDescriptor);
    pnoiLEDCharacteristics.setCallbacks(new LEDCharacteristicCallBack);

    ble_service->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
    ble_server->getAdvertising()->start();
    is_ble_connecting = true;
    Serial.println("Waiting a client connection to notify...");

    glow_white();
}
