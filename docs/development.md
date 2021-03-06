# Development

## Checking out this repository

Make sure you clone with submodules:

```bash
git clone --recurse-submodules {URL}
```

## Preparing your environment

Install [ESP-IDF SDK v4.4](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/get-started/index.html).

## Building and testing

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

### Build, flash and monitor

You can run all the above actions in series with 1 command:

```bash
idf.py -p $DEVICE build flash monitor
```

You can exit the monitor screen by pressing `CTRL` + `]`.

### Stacktrace analysis

```bash
xtensa-esp32-elf-addr2line -pfiaC -e build/OpenPyrojet.elf {ADDRESSES}
```

Where `{ADDRESSES}` is the series of hex addresses of the stack trace, after "Backtrace:" in the log.

## Configure the firmware project

**NOTE: `./sdkconfig` is checked and is configured correctly.
The items below are just a reference.**

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

## Skipping the WiFi setup

After each flash, the ESP32 starts the Access Point to configure WiFi. This makes development slow.
To avoid it, open folder `main/config/` and copy `default.json` into a new file named `user.json`.
Fill in the WiFi SSID & password in `user.json`.
