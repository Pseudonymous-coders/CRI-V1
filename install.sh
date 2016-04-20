echo "Welcome to the CRI installer
Created By: $AUTHORS
Version: $VERSION
Url: $URL

SYSTEM:
User: $USER
Arch: $ARCH
"

printf "Getting vars..."
export $CGLOBS=~/Downloads/.tmp/globs
export URL="https://raw.githubusercontent.com/Pseudonymous-coders/CRI/master"
sudo mkdir -p $CGLOBS
cd $CGLOBS
sudo wget -q --no-check-certificate "$URL/globvar" -O $CGLOBS/globvar
suod chmod 755 *
printf "Done\n"

source $CGLOBS/globvar

lineCount() #Same function called earlier in the previous script to use in the counting of lines in commands.txt
{
    wc -l < installLIST.txt	
}

clean() 
{
    tput cuu 1 && tput el
}

progressfilt ()
{
    local flag=false c count cr=$'\r' nl=$'\n'
    while IFS='' read -d '' -rn 1 c
    do
        if $flag
        then
            printf '%c' "$c"
        else
            if [[ $c != $cr && $c != $nl ]]
            then
                count=0
            else
                ((count++))
                if ((count > 1))
                then
                    flag=true
                fi
            fi
        fi
    done
}

export -f clean
export -f progressfilt

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
    ./${NAME##*/}
done



