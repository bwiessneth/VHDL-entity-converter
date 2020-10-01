#!/bin/sh

# Change to correct directory if working directory is somewhere else
SCRIPT_PATH=$( cd $(dirname $0) ; pwd )
cd "$SCRIPT_PATH"

export VERSION="0.2"

./install.sh AppDir

[ -f "linuxdeploy-x86_64.AppImage" ] || wget -O linuxdeploy-x86_64.AppImage https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage

mkdir -p AppDir/usr/share/vec
mv AppDir/etc/vec/vec.conf AppDir/usr/share/vec/vec.conf

./linuxdeploy-x86_64.AppImage --appdir AppDir
./linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage
