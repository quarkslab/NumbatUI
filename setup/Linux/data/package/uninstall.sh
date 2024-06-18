#!/bin/sh

if [ "$(id -u)" -ne 0 ]; then
  echo >&2 "Error: Please run script as root or with sudo. Exiting..."
  exit 1
fi

echo "Do you want to uninstall NumbatUI?"

printf 'enter [y/n] '
read ans
case ${ans:=y} in [yY]*) ;; *) exit ;; esac

rm /usr/bin/numbatui
rm /usr/share/mime/packages/numbatui-mime.xml
rm /usr/share/applications/numbatui.desktop
rm /usr/share/icons/numbatui.png
rm /usr/share/icons/project-numbatui.png
rm /opt/numbatui -rf
update-mime-database /usr/share/mime
update-desktop-database

echo "Deinstallation complete."
