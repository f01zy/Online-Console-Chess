#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "Need a root." 
   exit 1
fi

declare -A repos=(
    ["FTXUI"]="https://github.com/ArthurSonzogni/FTXUI"
    ["nlohmann/json"]="https://github.com/nlohmann/json"
    ["socket.io-client-cpp"]="https://github.com/socketio/socket.io-client-cpp --recurse-submodules"
)

for repo in "${!repos[@]}"; do
    echo "Installing ${repo}..."
    
    git clone ${repos[$repo]} > /dev/null 2>&1
    
    dir=$(basename "${repo}")
    cd "${dir}" || exit 1
    
    mkdir -p build && cd build
    cmake .. > /dev/null 2>&1
    make -j$(nproc) install > /dev/null 2>&1
    
    cd ../..
    rm -rf "${dir}"
    
    echo "${repo} installed successfully."
    echo "----------------------------------"
done

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
