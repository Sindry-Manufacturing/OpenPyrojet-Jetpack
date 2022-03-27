# OpenPyrojet Jetpack

This is the print head firmware project, designed for ESP32.

## Development

See our [Development Docs](docs/development.md).

## Hardware Required

To run this firmware, you need an ESP32 dev board (e.g. ESP32-WROVER Kit) or ESP32 core board (e.g. ESP32-DevKitC).
The ESP32 must have 4MB of ROM.

## Getting Started

Once you have installed the firmware on your ESP32, Jetpack will appear as a Wi-Fi access point:

![wifi access points on iOS](docs/images/wifi-ap-list.png)

Connect to it using the password `openpyrojet`.

Once connected, click on the connection information to find out the "Router IP". It will look like `192.168.4.1`.

The next step is to open the web browser. Surf to `http://192.168.4.1`.
Make sure to substitute the address with the router IP from the Wi-Fi settings.

![wifi access points on iOS](docs/images/wifi-ap-form.png)

When you pressed save, you should see a popup that confirms it.

Now reboot the ESP32. It should connect to your Wi-Fi network.

You can now use [Jetpack.js](https://github.com/Sindry-Manufacturing/OpenPyrojet-Jetpack.js) to connect to the device from your phone/laptop/desktop.

[GNU General Public License Version 3](LICENSE.md)
