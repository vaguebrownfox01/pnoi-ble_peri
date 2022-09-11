#include "pnoi_ble.h"

void setup()
{
    // Start serial communication
    Serial.begin(115200);

    init_pnoi_ble();
}

int x = 0;
void loop()
{
    if (is_ble_connected)
    {
        delay(100);
    }

    if (!is_ble_connected && !is_ble_connecting)
    {
        init_pnoi_ble();
    }
}
