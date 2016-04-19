AUTHORS=["Eli Smith","David Smerkous"]
VERSION="0.0.1"
URL="https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master"
USER=$(whoami)
ARCH=$(uname -m)

PREF=/usr/local
LIB_PATH=$PREF/lib/CRI/
CONFIG_PATH=$PREF/etc/CRI/
CRI_DIR=$PREF/tmp/CRI/
DEST_DIR=$CRI_DIR/dest
PKG_PATH=$LIB_PATH/packages

echo "Welcome to the CRI installer
Created By: $AUTHORS
Version: $VERSION
Url: $URL

SYSTEM:
User: $USER
Arch: $ARCH
"

lineCount() #Same function called earlier in the previous script to use in the counting of lines in commands.txt
{
    wc -l < installLIST.txt	
}

if [ $ARCH != "i686" ] && [ $ARCH != "x86_64" ] # Check if chromebook is compatible
then
  printf "Your device doesn't support CRI yet\nExiting..."
  sleep 0.5
  exit 1
fi

echo "Creating working directories..."
sudo mkdir -p $LIB_PATH $CONFIG_PATH $CONFIG_PATH/meta $CRI_DIR $DEST_DIR $PKG_PATH # Create new directories
sudo chown -R $USER:$USER $LIB_PATH $CONFIG_PATH $CONFIG_PATH/meta $CRI_DIR $DEST_DIR $PKG_PATH # Change ownership

PKGURL=$URL/chromelib

cd $CRI_DIR
printf "\nDownloading CRI files\n\n..." 
sudo wget -q --no-check-certificate "$URL/installLIST.txt" -O $CRI_DIR/installLIST.txt #This is to download list of files needed
sudo chmod 755 installLIST.txt #Makes the commands file have every permisson so that anyone can use it 
NAMES="$(< installLIST.txt)" #names from names.txt file
LINES=$(lineCount)
NUMBERS=1

cd $DEST_DIR

echo "$NAMES"

for NAME in $NAMES; do #Downloads all nessisary files from github to /usr/local/bin
    clean
    echo "File $NUMBERS/$LINES... ${NAME##*/}"
    let "NUMBERS += 1"
    sudo wget -q --no-check-certificate "$URL/$NAME" -O $DEST_DIR/${NAME##*/}
    sudo chmod 755 ${NAME##*/}
done



