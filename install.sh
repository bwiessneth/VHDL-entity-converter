#!/bin/bash

# Change to correct directory if working directory is somewhere else
SCRIPT_PATH=$( cd $(dirname $0) ; pwd )

INSTALL_DIR="/"

BIN="usr/bin"
APPLICATIONS="usr/share/applications"
ICONS="usr/share/icons/hicolor/256x256/apps"
CONFIG="etc/vec"

# make sure the project is built
make -j 4 || exit 1

# check for help flags
[ "$1" = "-h" ] || [ "$1" = "--help" ] && echo \
"Usage: ./install.sh [INSTALL_DIR]
Install VEC on a Linux system
Default INSTALL_DIR is the system root /

  -h, --help  display this help and exit
"

# set INSTALL_DIR to first argument if it exists
[ -z "$1" ] || INSTALL_DIR="$1"

# make sure all dirs are available
mkdir -p "$INSTALL_DIR"
cd "$INSTALL_DIR"
mkdir -p "$BIN"
mkdir -p "$APPLICATIONS"
mkdir -p "$ICONS"
mkdir -p "$CONFIG"

# copy files
cp "$SCRIPT_PATH/bin/VEC" "$BIN"
cp "$SCRIPT_PATH/assets/vec.desktop" "$APPLICATIONS"
cp "$SCRIPT_PATH/assets/vec.png" "$ICONS"
cp "$SCRIPT_PATH/assets/vec.conf" "$CONFIG"
