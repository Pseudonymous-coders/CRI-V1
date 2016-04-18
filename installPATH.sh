#!/bin/sh

echo "Starting install of all
depends in the TOBUILD folder
"
printf "fixing permissions..."
sudo chmod 755 *
sudo mount -i -o remount,exec /home/chronos/user
printf "DONE\n"

printf "starting library ....soooo installation..."
cd "1_LIBRARIES"
sudo sh init.sh
printf "DONE\n"

printf "starting dpkg base installation..."
cd "../2_DPKG"
sudo sh init.sh
printf "DONE\n"

printf "starting post gcc and stuff installation..."
cd "../3_POST"
sudo sh init.sh
printf "DONE\n"

printf "starting debs installation..."
cd "../4_DEBS"
#sudo ./init.sh
printf "DONE\n"