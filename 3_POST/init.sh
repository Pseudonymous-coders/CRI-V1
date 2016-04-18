#!/bin/bash

SBIN=/sbin
BIN=/bin

sudo chmod 755 *

echo "Init script for dpkg started

Moving daemon
"
sudo cp -r start-stop-daemon $SBIN/

echo "
Getting gcc5 base

"
sudo dpkg --force all --install gcc_5_base_first.deb

echo "
Building gawk

"
cd gawk
sudo chmod 755 *
sudo mount -i -o remount,exec /home/chronos/user
sudo ./configure --disable-sanity-check
sudo make install
sudo cp -r gawk $BIN/
cd ..

echo "
Building libc6

"
cd libc6
sudo chmod 755 *
sudo mkdir build
cd build
sudo ../configure --disable-sanity-check
sudo make install

cd ../../
sudo sh fix_dpkg

echo "
Getting gcc1 lib base

"
sudo dpkg --force all --install libgcc1.deb

echo "Finished installing files"