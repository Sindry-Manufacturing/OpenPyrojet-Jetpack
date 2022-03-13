# OpenPyrojet Jetpack

This is the print head firmware project, designed for ESP32.

## Hardware Required

To run this firmware, you need an ESP32 dev board (e.g. ESP32-WROVER Kit) or ESP32 core board (e.g. ESP32-DevKitC).
The ESP32 must have 4MB of ROM to be able to host all website files.

## Preparing your environment

1. Install [ESP-IDF SDK](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
2. Install NodeJS with `npm` by [downloading it manually](https://nodejs.dev/learn/how-to-install-nodejs) or by [using a package manager](https://nodejs.org/en/download/package-manager/)

## Configure the firmware project

From the project directory, run `idf.py menuconfig` to start configuring your ESP32.

In the `Serial flasher config` menu:

* `Flash Size` must be set to `4 MB` (make sure your ESP32 has 4MB of ROM!)
* Uncheck `Detect flash size when flashing bootloader`

In the `Partition Table` menu:

* Select `Custom partition CSV file` and pick `partitions.csv`

In the `Example Connection Configuration` menu:

* Choose the network interface in `Connect using`  option based on your board. Currently we support both Wi-Fi and Ethernet.
* If you select the Wi-Fi interface, you also have to set:
    * Wi-Fi SSID and Wi-Fi password that your esp32 will connect to.
* If you select the Ethernet interface, you also have to set:
    * PHY model in `Ethernet PHY` option, e.g. IP101.
    * PHY address in `PHY Address` option, which should be determined by your board schematic.
    * EMAC Clock mode, GPIO used by SMI.

In the `Example Configuration` menu:

* Set the domain name in `mDNS Host Name` option.
* Choose the deploy mode in `Website deploy mode`, currently we support deploy website to host PC, SD card and SPI Nor flash.
    * If we choose to `Deploy website to host (JTAG is needed)`, then we also need to specify the full path of the website in `Host path to mount (e.g. absolute path to web dist directory)`.
* Set the mount point of the website in `Website mount point in VFS` option, the default value is `/www`.

## Configure the web project

```bash
cd main/www
npm install
```

## Building and testing

### Build the website:

```bash
npm run build && rm dist/js/*.map
```

**Important**: The `*.map` files at `main/www/dist/js/` must be removed.
The website files won't fit on the ESP32 ROM with the map files present.

### Compile the project

```bash
idf.py build
```

### Flash the ROM to the ESP32

```bash
idf.py -p $DEVICE flash
```

Where `$DEVICE` looks something like:
- (macOS) `/dev/tty.SLAB_USBtoUART`
- (Linux) `/dev/ttyUSB0`

### Monitor the application

```bash
idf.py -p $DEVICE monitor
```

You can exit the monitor screen by pressing `CTRL` + `]`.

## License

[GNU General Public License Version 3](LICENSE.md)
