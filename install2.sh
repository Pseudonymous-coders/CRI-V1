printf "Getting vars...\n\n"
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

if ask "Have you successfully rootmounted yet?"; then
echo "Good! If you encounter 'Read only filesystem errors'
then please rootmount again"
else
echo "Exiting..."
exit 1
fi

echo "Thanks for installing CRI MATES!"

