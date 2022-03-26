# Usage:
#   flash.sh [port]
#
# Arguments:
#   port - the port of the device (e.g. COM1, /dev/ttyUSB0, ...)
#
# Requirements:
#   jq - run 'pip install jq'
#   esptool.py - run 'pip install esptool'
#
# Documentation:
#   https://docs.espressif.com/projects/esptool/en/latest/esp32/
#

# Source: https://stackoverflow.com/a/53798785
function is_bin_in_path {
  if [[ -n $ZSH_VERSION ]]; then
    builtin whence -p "$1" &> /dev/null
  else # Bash
    builtin type -P "$1" &> /dev/null
  fi
}

# Validate environment: tools
is_bin_in_path jq || echo jq not found - install python and run \'pip install jq\'
is_bin_in_path esptool.py || echo esptool.py not found - install python and run \'pip install esptool\'

# Validate environment: build files available
if [ ! -f "build/OpenPyrojet.bin" ]; then
  echo "ERROR: You must run 'idf.py build' first!"
  exit 1
fi

# Ensure a clean target folder
# Keep the folder itself, so you can keep the folder open during development
rm -rf build/package/*

# Create target directory tree
mkdir -p build/package
mkdir -p build/package/bootloader
mkdir -p build/package/partition_table

# Copy partitions
cp build/bootloader/bootloader.bin build/package/bootloader/
cp build/partition_table/partition-table.bin build/package/partition_table/
cp build/www.bin build/package/
cp build/config.bin build/package/
cp build/OpenPyrojet.bin build/package/

# Create flash command based on partitions
KEY_VALUES=`jq -r '.flash_files  | keys[] as $k | "\($k) \(.[$k])"' build/flasher_args.json  | tr "\n" " "`
LINUX_COMMAND="esptool.py --port \$1 write_flash $KEY_VALUES"
WINDOWS_COMMAND="esptool.py --port %1 write_flash $KEY_VALUES"
echo $LINUX_COMMAND > build/package/flash.sh
chmod +x build/package/flash.sh
echo $WINDOWS_COMMAND > build/package/flash.bat
