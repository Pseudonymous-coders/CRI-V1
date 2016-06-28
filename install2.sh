#!/bin/bash
echo "Testing internet connection..."
TEST=0
ping -q -w 1 -c 1 `ip r | grep default | cut -d ' ' -f 3` > /dev/null && TEST=1 || TEST=0   
if [ "$TEST" -eq "1" ]; then 
    echo "Connection established!"
else
    echo "Failed to connect to the internet!
Exiting..."
    exit 1
fi

echo "Getting vars...
"
CGLOBS=~/Downloads/.tmp/globs
CTEMP=~/Downloads/.tmp
URL="https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master"
UPFOLDER=/home/chronos/user/Downloads/.tmp/cridate
CFGFILE=$UPFOLDER/cri.cfg
sudo mkdir -p $CGLOBS ~/Downloads/.tmp
cd $CGLOBS
sudo wget -q --no-check-certificate "$URL/globs/globvar" -O $CGLOBS/globvar # Update variables
sudo wget -q --no-check-certificate "$URL/globs/globfun" -O $CGLOBS/globfun # Update functions
sudo chmod 755 *
printf "Done\n\n\n"
clear
source globvar
source globfun

echo "Welcome to PART 2 of the CRI installer!
Created By: $AUTHORS
Version: $VERSION
Url: $URL

"
sleep 1

printf "Checking root mount..."
sudo mkdir /critest 2&>/dev/null
if [ ! -d /critest ]; then
    printf "\n\n" 
    if ask "Have you successfully rootmounted yet?"; then
        printf "It doesn't look like it :'(...\n\n If you encounter 'Read only filesystem errors'\nthen please rootmount again\n"
    else
        printf "\n\n"
        echo "Exiting..."
        sleep 2
        exit 1
    fi
else 
    echo "DONE (mounted correctly)"
fi
sudo rm -rf /critest 2&>/dev/null

printf "Creating working directories..."
sudo mkdir -p $CTEMP $CPKG $CBUILD 2&>/dev/null
sudo chown $USER:$USER $CTEMP $CPKG $CBUILD 2&>/dev/null
echo "DONE"
PKGURL="$URL/chrootlib"

cd $CTEMP
printf "\nDownloading CHROOT files..." 
{ sudo wget -q --no-check-certificate "$URL/install.sh" -O $CTEMP/install.sh
sudo chmod 755 install.sh
sudo mount -o remount,exec /home/chronos/user -i
sudo wget -q --no-check-certificate "$URL/chrootLIST.txt" -O $CTEMP/chrootLIST.txt #This is to download list of files needed
sudo chmod 755 chrootLIST.txt 
} >/dev/null 2>&1 #Makes the commands file have every permisson so that anyone can use it 
echo "DONE"

NAMES="$(< chrootLIST.txt)" #names from names.txt file
LINES=$(chrootCount)
NUMBERS=1

cd $CPKG
LOCKATION=$CROUTON/usr/bin/ # Feed location

for NAME in $NAMES; do #Downloads all nessisary files from github to /usr/local/bin
    clear
    printf "Welcome to the CRI installer!\nCreated By: $AUTHORS\nVersion: $VERSION\nFile $NUMBERS/$LINES...\n\n ${NAME##*/} \n"
    let "NUMBERS += 1"
    echo "Installing ${NAME##*/} 
    "
    installer "$PKGURL/$NAME" "$LOCKATION/${NAME##*/}"
    sudo chmod 755 $LOCKATION/* 2&>/dev/null
    sudo chown $USER:$USER $LOCKATION/${NAME##*/} 2&>/dev/null
    fixowner 2&>/dev/null
    printf "\n\nDONE\n"
    sleep 2
done

clear

echo "Done installing chroot files
Installing new items
"
sudo mkdir $CTEMP 2&>/dev/null;
echo "" > $CTEMP/coms;
sudo chown $USER:$USER $CTEMP/coms 2&>/dev/null

clear

echo "Installing icons for file manager..."
sudo writer "printf 'y\ny\ny\n' % apt-get install dialog thunar gnome-icon-theme-extras gnome-icon-theme-full git"
sleep 0.5 # Wait for file update
sudo enter-chroot -u root runner # Run the above multi command
printf "\n\n\nDone\n\nInstalling mysql...\n"

clear

sudo writer "printf 'y\ny\n' % apt-get install mysql-client"
sleep 0.5
sudo enter-chroot -u root runner

echo "
Done

Installing webserver items...
"
sudo writer "service apache2 stop 2&>/dev/null+update-rc.d -f apache2 remove 2&>/dev/null+printf 'y\ny\n' % apt-get remove apache2"
sleep 0.5
sudo enter-chroot -u root runner

echo "Part 2..."

sudo writer "printf 'y\ny\ny\n' % apt-get install lighttpd+service lighttpd start 2&>/dev/null+service lighttpd force-reload+sudo rm -rf /var/www/index.lighttpd.html "
sleep 0.5
sudo enter-chroot -u root runner

cd $CROUTON/var/www

sudo git config --global user.email "pseudonymous.coders@gmail.com"
sudo git config --global user.name "Pseudonymous_Coders"
sudo git init
sudo git remote add -f origin "https://github.com/Pseudonymous-coders/CRI.git"
sudo git config core.sparseCheckout true
sudo chown -R $USER:$USER ../
sudo chmod -R 755 ../
sudo echo "localSite/" >> .git/info/sparse-checkout
git pull --depth=1 origin master
sudo rm -rf .git
sudo mv localSite/* .
sudo rm -rf localSite

echo "
Finished setting up local server. Installing webpysock Server...
"

clear

sudo writer "printf 'y\ny\ny\n' % apt-get install python python-dev python-pycurl python-simplejson sqlite3 python-pip build-essential python-tornado % sudo rm -rf *"
sleep 0.5
sudo enter-chroot -u root runner
sleep 0.5

clear

sudo writer "pip install --upgrade pip+pip install --upgrade virtualenv+"
sleep 0.5
sudo enter-chroot -u root runner
# Since we want http://cri/ we need to add them to the hosts
if [[ ! -z $(grep ' cri' '/etc/hosts') ]];  then echo "Cri already in host file"; else sudo su -c "sudo echo '127.0.0.1       cri' >> /etc/hosts"; fi
if [[ ! -z $(grep ' cri' "$CROUTON/etc/hosts") ]];  then echo "cri already in chroot host file"; else sudo su -c "sudo echo '127.0.0.1       cri' >> $CROUTON/etc/hosts"; fi

clear

echo "Double checking...
"
sleep 1

if [[ ! -z $(grep ' cri' '/etc/hosts') ]];  then echo "cri already in host file"; else sudo su -c "echo '127.0.0.1       cri' >> /etc/hosts"; fi
if [[ ! -z $(grep ' cri' "$CROUTON/etc/hosts") ]];  then echo "cri already in chroot host file"; else sudo su -c "echo '127.0.0.1       cri' >> $CROUTON/etc/hosts"; fi

#sudo writer 'if '+"[["+' ! -z $(grep "cri" "/etc/hosts") '+"]]"+'+  then echo "cri already in host file"+ else echo "127.0.0.1       cri" >> /etc/hosts"+ fi'

echo "Installing themes...
Please wait a bit.
"

sudo writer "printf 'y\ny\ny\n' % apt-get install software-properties-common python3-software-properties unzip file-roller"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Getting fonts..."
cd $CTEMP
sudo writer "mkdir -p /usr/share/fonts/truetype/ttf-ubuntu+wget http://font.ubuntu.com/download/ubuntu-font-family-0.83.zip -O ~/font.zip+cd+printf 'A\ny\na\n' % unzip font.zip+cd ubuntu-font-family-0.83+cp -r * /usr/share/fonts/truetype/ttf-ubuntu/"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Configuring i3..."
sudo writer "wget https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/chrootlib/xiwi.conf -O /etc/crouton/xiwi.conf"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Getting nice looking terminal..."
sudo writer "printf 'y\ny\ny\n' % apt-get install gnome-terminal+gconftool-2 --set /apps/gnome-terminal/profiles/Default/use_system_font --type boolean false+gconftool-2 --set /apps/gnome-terminal/profiles/Default/font --type string \"Ubuntu Mono 14\"+gconftool-2 --set /apps/gnome-terminal/profiles/Default/title --type string CRIterm" # Add gnome terminal
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Updating nice lookin' terminal reinit settings..."
sudo writer "gconftool-2 --set /apps/gnome-terminal/profiles/Default/title_mode --type string after"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Please customize your theme!"
sudo writer "printf '\n\n\n\n' % add-apt-repository ppa:moka/stable+sleep 1+chmod -R 777 /var/+printf '\n\n\n\n' % add-apt-repository ppa:numix/ppa+sleep1+printf 'y\n\n' % apt-get update+printf 'y\ny\ny\n' % apt-get install numix-gtk-theme moka-gtk-theme numix-icon-theme numix-icon-theme-circle moka-icon-theme lxappearance"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Double checking..."
sudo writer "printf 'y\ny\ny\n' % apt-get install numix-gtk-theme moka-gtk-theme numix-icon-theme numix-icon-theme-circle moka-icon-theme lxappearance+sleep 0.5+xiwi lxappearance"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Fixing home environment..."
sudo writer "echo \"export HOME=/root/\" >> /root/.bashrc"
sleep 0.5
sudo enter-chroot -u root runner

clear

echo "Adding to start up script..."
sudo writer "wget https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/chrootlib/cri -O /etc/init.d/cri+chmod 755 /etc/init.d/cri+sleep 0.1+update-rc.d -f cri remove+sleep 1+update-rc.d cri defaults+sudo echo 'PATH=\$PATH:/usr/games/' >> ~/.bashrc"
sleep 0.5
sudo enter-chroot -u root runner
echo "Added cri to the chroot startup!"

sudo writer "wget \"$COMURL/updatecri\" --no-check-certificate -q -O /usr/bin/updatecri > /dev/null 2>&1+chmod 755 -R /usr/bin/updatecri /var/www/ > /dev/null 2>&1+chown root:root -R /usr/bin/updatecri /var/www/ > /dev/null 2>&1"
sleep 0.5
sudo enter-chroot -u root runner

echo "Downloading startup script for ChromeOS..."
sudo wget "https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/com/cri.conf" --no-check-certificate -O /etc/init/cri.conf
echo "DONE"

clear

echo "Adding update files..."
sudo mkdir -p $UPFOLDER

if [ ! -d "$UPFOLDER" ]; then sudo mkdir -p $UPFOLDER; fi

sudo chown -R $USER:$USER $UPFOLDER
sudo chmod -R 755 $UPFOLDER

sleep 1
sudo apt-get install cmatrix
sudo chown $USER:$USER $CFGFILE
sudo chmod 755 $CFGFILE

if [ ! -e "$CFGFILE" ]; then
    echo "Continue..."
    sudo su -c "touch $CFGFILE"
    sudo su -c "echo \"VERSION0.0ENDVERSION...NAMEnoneENDNAME...DATE1/1/16ENDDATE\" > $CFGFILE"
    sudo chown $USER:$USER $CFGFILE
    sudo chmod 755 $CFGFILE
    sudo touch $CFGFILE
    sudo echo "VERSION0.0ENDVERSION...NAMEnoneENDNAME...DATE1/1/16ENDDATE" > $CFGFILE
fi
sudo apt-get install espeak
sudo echo "$(sudo wget "https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/globs/cri.cfg" --no-check-certificate -q -O -)" > /home/chronos/user/Downloads/.tmp/cridate/cri.cfg 
sudo su -c "echo $(sudo wget https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master/globs/cri.cfg --no-check-certificate -q -O -) > /home/chronos/user/Downloads/.tmp/cridate/cri.cfg"
sudo mount -o remount,exec /home/chronos/user -i
espeak "Thanks for installing CRI!"

cmatrix

