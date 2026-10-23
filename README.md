# Connect an ESP32 to Wi-Fi with the ESP SoftAP Prov app

An Arduino library for ESP32 boards that makes it easy to connect to the wifi network with the official ESP SoftAP prov app. The app helps to give the device the wifi credentials and stores it to the ESP32 memory. All esp arduino supported boards are supported with this library too.

This guide shows the simple setup flow for the `wifi_connectapp` Arduino library. It follows the idea of Espressif's [WiFiProv example](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiProv/examples/WiFiProv), with the key steps explained in plain language.

## What is provisioning?

Your ESP32 needs the name (SSID) and password of your home Wi-Fi network. Instead of putting those details in the sketch, the ESP32 temporarily opens its own Wi-Fi setup connection. The ESP SoftAP Prov app sends your home Wi-Fi details to the ESP32. The board then joins your home network and remembers the credentials in flash.

## The app screen

![ESP SoftAP Prov app start screen. Tap “Provision New Device” to begin setting up an ESP32.](doc/Screenshot%202026-10-23%20153052.png)

The pictured screen is the app's starting page. Tap **Provision New Device** when your ESP32 is powered on and running a sketch that calls `startWifi()`.

## What happens next

1. The ESP32 starts a temporary Wi-Fi access point for provisioning (SoftAP).
2. In the app, select the device / provisioning network it finds. The default network name in this library is `my_device`.
3. Enter the provisioning password if the app asks for it. The library's current default is `password`. If the app asks for a Proof of Possession (PoP), enter `abcd1234` (the current library default).
4. Enter the SSID and password of the Wi-Fi network you want the ESP32 to use.
5. The app sends those credentials to the ESP32. The ESP32 disconnects from its temporary setup network and connects to your Wi-Fi router.
6. When connected, the Serial Monitor prints the assigned IP address.

> The provisioning password (`service_key`) protects the temporary setup connection. The Proof of Possession (`pop`) is an additional device authentication value. This library currently sets it to `abcd1234`. Keep these values private for products you distribute, and change the defaults in `src/wifi_connectapp.cpp` before building your sketch.

## Minimal Arduino sketch

Install this library and select an ESP32 board supported by the Arduino-ESP32 core. Then use the library to start provisioning:

```cpp
#include <WiFi.h>
#include <wifi_connectapp.h>

wifi_connectapp provisioner;

void setup() {
  Serial.begin(115200);
  provisioner.startWifi();

  // Wait for the ESP32 to join the Wi-Fi network entered in the app.
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
  }

  Serial.println("\nWi-Fi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Your application code goes here.
}
```

Open the Serial Monitor at **115200 baud** to see provisioning and connection messages. Keep the board powered while entering the Wi-Fi details in the app.

## Configuration in this library

The current defaults are in [`src/wifi_connectapp.cpp`](src/wifi_connectapp.cpp):

| Setting | Current value | Purpose |
| --- | --- | --- |
| `service_name` | `my_device` | Name of the temporary provisioning access point |
| `service_key` | `password` | Password for the provisioning access point |
| `pop` | `abcd1234` | Proof of Possession used to authenticate the device |

This library starts provisioning over **SoftAP** and uses the Arduino-ESP32 `WiFiProv` API. It is not the same as connecting directly to your router: the phone app first passes the router credentials to the ESP32.

## If it does not connect

- Confirm the phone is using the **ESP SoftAP Prov** app and follow its prompts after tapping **Provision New Device**.
- Check the Serial Monitor for `Provisioning started`, credential, failure, and IP address messages.
- Make sure the phone enters the correct Wi-Fi SSID and password. The ESP32 must be able to reach that network.
- To clear stored network settings and try setup again, call `wifi_connectapp::eraseNVS()` from your sketch, then restart the ESP32. Erasing NVS clears saved settings, so use it only when you want a fresh setup.

For the lower-level Espressif example and API details, see [Provisioning for Arduino (WiFiProv)](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiProv/examples/WiFiProv).



