#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "Need a root." 
   exit 1
fi

git clone --recurse-submodules https://github.com/socketio/socket.io-client-cpp.git
cd socket.io-client-cpp || exit 1
cmake .
make install
cd ..
rm -rf socket.io-client-cpp

install_libcurl() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [ -f /etc/debian_version ]; then 
            apt install -y libcurl4-openssl-dev
        elif [ -f /etc/arch-release ]; then
            pacman -S --noconfirm curl
        else
            echo "Unknown OS."
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install curl
    else
        echo "Unknown OS."
    fi
}

install_libcurl
