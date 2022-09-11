#include "pnoi_ble.h"

// LED Characteristic and Descriptor
BLECharacteristic pnoiLEDCharacteristics(BLE_LED_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor pnoiLEDDescriptor(BLE_LED_DESCRIPTOR_UUID);

bool is_ble_connected = false;
bool is_ble_connecting = false;

class CharacteristicCallBack : public BLECharacteristicCallbacks
{
public:
    // This method not called
    void onWrite(BLECharacteristic *characteristic_) override
    {

        std::string d = characteristic_->getValue();
        size_t dl = characteristic_->getLength();

        const char *action = d.c_str();
        switch (get_action(action))
        {
        case start:
            digitalWrite(LED_PIN_RED, LOW);
            digitalWrite(LED_PIN_GREEN, HIGH);
            digitalWrite(LED_PIN_BLUE, LOW);
            break;

        case stop:
            digitalWrite(LED_PIN_RED, LOW);
            digitalWrite(LED_PIN_GREEN, LOW);
            digitalWrite(LED_PIN_BLUE, HIGH);
            break;

        case error:
            digitalWrite(LED_PIN_RED, HIGH);
            digitalWrite(LED_PIN_GREEN, LOW);
            digitalWrite(LED_PIN_BLUE, LOW);
            break;

        default:
            digitalWrite(LED_PIN_RED, HIGH);

            break;
        }

        Serial.println(d.c_str());
    }
};

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        is_ble_connected = true;
        is_ble_connecting = false;
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_GREEN, LOW);
        digitalWrite(LED_PIN_BLUE, HIGH);
    };

    void onDisconnect(BLEServer *pServer)
    {
        is_ble_connected = false;
    }
};

void init_pnoi_ble()
{

    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);

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
    is_ble_connecting = true;
    Serial.println("Waiting a client connection to notify...");

    digitalWrite(LED_PIN_RED, HIGH);
    digitalWrite(LED_PIN_GREEN, HIGH);
    digitalWrite(LED_PIN_BLUE, HIGH);
}

pnoi_action_code get_action(std::string const &inString)
{
    if (inString == "start")
        return start;
    if (inString == "stop")
        return stop;
    if (inString == "error")
        return error;
};
