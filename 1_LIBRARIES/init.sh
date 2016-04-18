#!/bin/bash

LIBFOLDER=/usr/lib/
LIBSIXFOUR=/usr/lib64/

sudo chmod 755 *

echo "Init script for libraries started

Moving all libs to 64
"
sudo cp -r lib* $LIBFOLDER

echo "
Moving all libs to 32

"
sudo cp -r lib* $LIBSIXFOUR

echo "
Moving gcc and dpkg libs
"
sudo cp -r gcc $LIBFOLDER
sudo cp -r dpkg $LIBFOLDER
sudo cp -r apt $LIBFOLDER

echo "
Moving make libs
"

sudo cp -r cmake $LIBSIXFOUR

echo "Finished copying files"