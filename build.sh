# Usage:
#   ./build.sh /dev/ttyUSB0

cd main/www
npm run build
rm dist/js/*.map
cd -

idf.py build
idf.py -p $1 flash
idf.py -p $1 monitor
