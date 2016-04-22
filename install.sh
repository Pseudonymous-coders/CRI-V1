printf "Getting vars...\n\n"
CGLOBS=~/Downloads/.tmp/globs
URL="https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master"
sudo mkdir -p $CGLOBS ~/Downloads/.tmp
cd $CGLOBS
sudo wget -q --no-check-certificate "$URL/globs/globvar" -O $CGLOBS/globvar
sudo wget -q --no-check-certificate "$URL/globs/globfun" -O $CGLOBS/globfun
sudo chmod 755 *
printf "Done\n\n\n"
clear
source globvar
source globfun

echo "Welcome to the CRI installer
Created By: $AUTHORS
Version: $VERSION
Url: $URL

SYSTEM:
User: $USER
Arch: $ARCH
"

sleep 1


if [ $ARCH != "i686" ] && [ $ARCH != "x86_64" ] # Check if chromebook is compatible
then
  printf "Your device doesn't support CRI yet\nExiting..."
  sleep 0.5
  exit 1
fi

if ask "This installation will take up to 25 minutes (Depending on connection) do you want to continue"; then
    echo "Continuing"
else
    echo "Exiting..."
    exit 1
fi

echo "Creating working directories..."
sudo mkdir -p $CTEMP $CPKG $CBUILD
sudo chown $USER:$USER $CTEMP $CPKG $CBUILD
#sudo mkdir -p $LIB_PATH $CONFIG_PATH $CONFIG_PATH/meta $CRI_DIR $DEST_DIR $PKG_PATH # Create new directories
#sudo chown -R $USER:$USER $LIB_PATH $CONFIG_PATH $CONFIG_PATH/meta $CRI_DIR $DEST_DIR $PKG_PATH # Change ownership

PKGURL=$URL/chromelib

cd $CTEMP
printf "\nDownloading CRI files\n\n..." 
sudo wget -q --no-check-certificate "$URL/install.sh" -O $CTEMP/install.sh
sudo chmod 755 install.sh
sudo mount -o remount,exec /home/chronos/user -i
sudo wget -q --no-check-certificate "$URL/installLIST.txt" -O $CTEMP/installLIST.txt #This is to download list of files needed
sudo chmod 755 installLIST.txt #Makes the commands file have every permisson so that anyone can use it 
NAMES="$(< installLIST.txt)" #names from names.txt file
LINES=$(lineCount)
NUMBERS=1

cd $CPKG

for NAME in $NAMES; do #Downloads all nessisary files from github to /usr/local/bin
    clear
    printf "Welcome to the CRI installer\nCreated By: $AUTHORS\nVersion: $VERSION\nFile $NUMBERS/$LINES...\n\n ${NAME##*/} \n"
    let "NUMBERS += 1"
    sudo wget -q --no-check-certificate "$PKGURL/$NAME" -O $CPKG/${NAME##*/}
    sudo chmod 755 *
    sudo chown $USER:$USER ${NAME##*/}
    ./${NAME##*/} # Run setup in seperate thread 
    fixowner 2&>/dev/null
done

wait # Wait for threads to update

clear
clear
printf "Welcome to the CRI installer\nCreated By: $AUTHORS\nVersion: $VERSION\nFinal Steps... \n"

echo "These are going to be the final steps to installing CRI
The current problem is that chromebooks are sandboxed and we
need access to bin files. So we are going to change some system
flags... After this it'll ask you to reboot, it will probably fail
because only one partitioned was remounted. Until rootmount is says it was successful
in remounting, then you can stop rebooting... After that, run the install2.sh script
That will be located in your downloads folder. Like this >

REMOUNTING AS ROOT >
rootmount

INSTALLATION PART TWO>
sudo ~/Downloads/install2.sh

"

if ask "Continue"; then; fi

rootmount

echo "Cleaning up everything...
"

sudo rm -rf $CPKG
sudo rm -rf $CBUILD

echo "Done

"
echo "Thanks for installing CRI, now you can open up the extension to run it..."
