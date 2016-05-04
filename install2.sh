#!/bin/bash

echo "Getting vars...
"
CGLOBS=~/Downloads/.tmp/globs
URL="https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master"
sudo mkdir -p $CGLOBS ~/Downloads/.tmp
cd $CGLOBS
sudo wget -q --no-check-certificate "$URL/globs/globvar" -O $CGLOBS/globvar # Update variables
sudo wget -q --no-check-certificate "$URL/globs/globfun" -O $CGLOBS/globfun # Update functions
sudo chmod 755 *
printf "Done\n\n\n"
clear
source globvar
source globfun

echo "Welcome to PART 2 of the CRI installer
Created By: $AUTHORS
Version: $VERSION
Url: $URL

"

if ask "Have you successfully rootmounted yet"; then
echo "Good! If you encounter 'Read only filesystem errors'
then please rootmount again"
else
echo "Exiting..."
exit 1
fi

echo "Creating working directories..."
sudo mkdir -p $CTEMP $CPKG $CBUILD 2&>/dev/null
sudo chown $USER:$USER $CTEMP $CPKG $CBUILD 2&>/dev/null

PKGURL=$URL/chrootlib

cd $CTEMP
printf "\nDownloading CHROOT files\n\n..." 
sudo wget -q --no-check-certificate "$URL/install.sh" -O $CTEMP/install.sh
sudo chmod 755 install.sh
sudo mount -o remount,exec /home/chronos/user -i
sudo wget -q --no-check-certificate "$URL/chrootLIST.txt" -O $CTEMP/chrootLIST.txt #This is to download list of files needed
sudo chmod 755 chrootLIST.txt #Makes the commands file have every permisson so that anyone can use it 
NAMES="$(< chrootLIST.txt)" #names from names.txt file
LINES=$(chrootCount)
NUMBERS=1

cd $CPKG
LOCKATION=$CROUTON/usr/bin

for NAME in $NAMES; do #Downloads all nessisary files from github to /usr/local/bin
    clear
    printf "Welcome to the CRI installer\nCreated By: $AUTHORS\nVersion: $VERSION\nFile $NUMBERS/$LINES...\n\n ${NAME##*/} \n"
    let "NUMBERS += 1"
    sudo wget -q --no-check-certificate "$PKGURL/$NAME" -O $LOCKATION/${NAME##*/}
    sudo chmod 755 $LOCKATION/*
    sudo chown $USER:$USER $LOCKATION/${NAME##*/}
    fixowner 2&>/dev/null
done

echo "Thanks for installing CRI MATES!"

