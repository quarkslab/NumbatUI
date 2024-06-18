#!/bin/sh

if [ "$(id -u)" -ne 0 ]; then
  echo >&2 "Error: Please run script as root or with sudo. Exiting..."
  exit 1
fi

echo "Installing NumbatUI in /opt/numbatui"

MY_PATH="$( cd "$( dirname "$0" )" && pwd )"

rm -rf /opt/numbatui > /dev/null

cp -rf "$MY_PATH/" /opt/numbatui > /dev/null
cp -rf "$MY_PATH/share/applications" /usr/share > /dev/null
cp -rf "$MY_PATH/share/icons" /usr/share > /dev/null
cp -rf "$MY_PATH/share/mime" /usr/share > /dev/null

cp "$MY_PATH/share/data/gui/icon/logo_1024_1024.png" /usr/share/icons/numbatui.png > /dev/null
cp "$MY_PATH/share/data/gui/icon/project_256_256.png" /usr/share/icons/project-numbatui.png > /dev/null

ln -f -s /opt/numbatui/NumbatUI.sh /usr/bin/numbatui

update-mime-database /usr/share/mime > /dev/null
update-desktop-database > /dev/null

mkdir -p ~/.config/numbatui
if [ "$SUDO_USER" ]; then CHOWNUSER=$SUDO_USER; else CHOWNUSER=$USER; fi
chown -R $CHOWNUSER ~/.config/numbatui

echo "Installation complete."
echo "Enter 'numbatui' to launch application."
