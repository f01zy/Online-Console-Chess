#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "Need a root." 
   exit 1
fi

echo "Installing nlohmann/json"
git clone https://github.com/nlohmann/json > /dev/null 2>&1
cd json || exit 1
mkdir build

echo "Configuring socket.io-client-cpp with CMake..."
cmake .. > /dev/null 2>&1

echo "Building and installing socket.io-client-cpp..."
make install > /dev/null 2>&1
cd ../..
rm -rf json

echo "nlohmann/json installed successfully."

echo "Installing socket.io-client-cpp..."
git clone --recurse-submodules https://github.com/socketio/socket.io-client-cpp.git > /dev/null 2>&1
cd socket.io-client-cpp || exit 1

echo "Configuring socket.io-client-cpp with CMake..."
cmake . > /dev/null 2>&1

echo "Building and installing socket.io-client-cpp..."
make install > /dev/null 2>&1
cd ..
rm -rf socket.io-client-cpp

echo "socket.io-client-cpp installed successfully."

install_libcurl() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [ -f /etc/debian_version ]; then 
            echo "Installing libcurl for Debian-based systems..."
            apt install -y libcurl4-openssl-dev > /dev/null 2>&1
        elif [ -f /etc/arch-release ]; then
            echo "Installing libcurl for Arch-based systems..."
            pacman -S --noconfirm curl > /dev/null 2>&1
        else
            echo "Unknown OS."
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "Installing libcurl for macOS..."
        brew install curl > /dev/null 2>&1
    else
        echo "Unknown OS."
    fi
    echo "libcurl installed successfully."
}

install_libcurl
