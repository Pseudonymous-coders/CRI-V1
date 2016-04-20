printf "Getting vars...\n\n"
CGLOBS=~/Downloads/.tmp/globs
URL="https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master"
sudo mkdir -p $CGLOBS ~/Downloads/.tmp
cd $CGLOBS
sudo wget -q --no-check-certificate "$URL/globs/globvar" -O $CGLOBS/globvar
sudo wget -q --no-check-certificate "$URL/globs/globvar" -O $CGLOBS/globfun
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

if [ $ARCH != "i686" ] && [ $ARCH != "x86_64" ] # Check if chromebook is compatible
then
  printf "Your device doesn't support CRI yet\nExiting..."
  sleep 0.5
  exit 1
fi

echo "Creating working directories..."
sudo mkdir -p $CTEMP $CPKG $CBUILD
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
    clean
    echo "File $NUMBERS/$LINES... ${NAME##*/}"
    let "NUMBERS += 1"
    sudo wget -q --no-check-certificate "$PKGURL/$NAME" -O $CPKG/${NAME##*/}
    sudo chmod 755 ${NAME##*/}
    ./${NAME##*/} # Run setup
done

echo "Thanks for installing CRI, now you can open up the extension to run it..."


