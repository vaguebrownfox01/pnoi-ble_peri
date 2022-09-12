#include "pnoi_ble.h"
#include "pnoi_ext.h"

void setup()
{
    // Start serial communication
    Serial.begin(115200);

    init_pnoi_ext();
    init_pnoi_ble();
}

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
