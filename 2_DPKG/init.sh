#!/bin/bash

VARFOL=/var/lib
ETCFOL=/etc
USRLIBFOL=/usr/lib
USRSHAREFOL=/usr/share
USRBINFOL=/usr/bin
USRSBINFOL=/usr/sbin

sudo chmod 755 *

echo "Init script for dpkg started

Moving all VAR files
"
cd varFol
sudo mkdir $VARFOL/dpkg
sudo cp -r * $VARFOL/dpkg

echo "
Moving all ETC files

"
cd ../etcFol
sudo mkdir $ETCFOL/cron.daily
sudo mkdir $ETCFOL/logrotate.d
sudo cp -r dpkg $ETCFOL/

echo "
Moving all LIB files

"
cd ../usrLibFol
sudo cp -r dpkg $USRLIBFOL/

echo "
Moving all SHARE files

"
cd ../usrShareFol
sudo cp -r dpkg $USRSHAREFOL/

echo "
Moving all BIN files

"
cd ../usrBinFol
sudo cp -r * $USRBINFOL/

echo "
Moving all SBIN files

"
cd ../usrSBinFol
sudo cp -r * $USRSBINFOL/

echo "Finished copying files"