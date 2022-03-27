rm build/package.zip
zip build/package.zip build/bootloader/bootloader.bin \
  build/partition_table/partition-table.bin \
  build/www.bin \
  build/config.bin \
  build/OpenPyrojet.bin \
  build/flasher_args.json \
  flash.ps1 \
  flash.sh
