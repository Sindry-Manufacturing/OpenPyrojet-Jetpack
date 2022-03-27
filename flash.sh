# Usage:
#   flash.sh [port] [folder]
#
# Arguments:
#   port - the port of the device (e.g. /dev/ttyUSB0, ...)
#   folder - folder where the build files can be found (optional)
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

cd build
# Create flash command based on partitions
KEY_VALUES=`jq -r '.flash_files  | keys[] as $k | "\($k) \(.[$k])"' flasher_args.json  | tr "\n" " "`
esptool.py --port $1 -b 921600 write_flash $KEY_VALUES
cd -
