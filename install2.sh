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
    echo "Installing ${NAME##*/} 
    "
    installer "$PKGURL/$NAME" "$LOCKATION/${NAME##*/}"
    sudo chmod 755 $LOCKATION/* 2&>/dev/null
    sudo chown $USER:$USER $LOCKATION/${NAME##*/} 2&>/dev/null
    fixowner 2&>/dev/null
    echo "Done"
    sleep 0.5
done

echo "Done installing chroot files
Installing new items
"
sudo mkdir $CTEMP 2&>/dev/null;
echo "" > $CTEMP/coms;
sudo chown $USER:$USER $CTEMP/coms 2&>/dev/null

echo "Installing icons for file manager..."
sudo writer "printf 'y\ny\ny\n' % apt-get install dialog thunar gnome-icon-theme-extras gnome-icon-theme-full git"
sleep 0.5 # Wait for file update
sudo enter-chroot -u root runner # Run the above multi command
printf "\n\n\nDone\n\nInstalling mysql...\n"


sudo writer "printf 'y\ny\n' % apt-get install mysql-client"

echo "
Done

Installing webserver items...
"
sudo writer "service apache2 stop 2&>/dev/null+update-rc.d -f apache2 remove 2&>/dev/null+printf 'y\ny\n' % apt-get remove apache2"
sleep 0.5
sudo enter-chroot -u root runner

echo "Part 2..."

sudo writer "printf 'y\ny\ny\n' % apt-get install lighttpd+service lighttpd start 2&>/dev/null+service lighttpd force-reload+sudo rm -rf /var/www/index.lighttpd.html"
sleep 0.5
sudo enter-chroot -u root runner

cd $CROUTON/var/www

sudo git init
sudo git remote add -f origin "https://github.com/Pseudonymous-coders/CRI.git"
sudo git config core.sparseCheckout true
sudo chown -R $USER:$USER ../
sudo chmod -R 755 ../
sudo echo "localSite/" >> .git/info/sparse-checkout
git pull --depth=1 origin master
sudo mv localSite/* .
sudo rm -rf localSite

echo "
Finished setting up local server, installing webpysock Server...
"

sudo writer "printf 'y\ny\ny\n' % apt-get install python python-dev python-pycurl python-simplejson sqlite3 python-pip build-essential python-tornado"
sleep 0.5
sudo enter-chroot -u root runner
sleep 0.5
sudo writer "pip install --upgrade pip+pip install --upgrade virtualenv+"

# Since we want http://cri/ we need to add them to the hosts
if [[ ! -z $(grep 'cri' '/etc/hosts') ]];  then echo "cri already in host file"; else sudo su -c "sudo echo '127.0.0.1       cri' >> /etc/hosts"; fi
if [[ ! -z $(grep 'cri' "$CROUTON/etc/hosts") ]];  then echo "cri already in chroot host file"; else sudo su -c "sudo echo '127.0.0.1       cri' >> $CROUTON/etc/hosts"; fi
#sudo writer 'if '+"[["+' ! -z $(grep "cri" "/etc/hosts") '+"]]"+'+  then echo "cri already in host file"+ else echo "127.0.0.1       cri" >> /etc/hosts"+ fi'
sleep 0.5
sudo enter-chroot -u root runner

echo "Thanks for installing CRI MATES!"

