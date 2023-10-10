#!/bin/sh
hostip=$(cat /etc/resolv.conf | grep nameserver | awk '{ print $2 }')
wslip=$(hostname -I | awk '{print $1}')
port=29758

PROXY_HTTP="http://${hostip}:${port}"

set_proxy(){
    export http_proxy="${PROXY_HTTP}"
    export HTTP_PROXY="${PROXY_HTTP}"

    export https_proxy="${PROXY_HTTP}"
    export HTTPS_proxy="${PROXY_HTTP}"
}

unset_proxy(){
    unset http_proxy
    unset HTTP_PROXY
    unset https_proxy
    unset HTTPS_PROXY
}

test_setting(){
    echo "Host ip:" ${hostip}
    echo "WSL ip:" ${wslip}
    echo "Current proxy:" $https_proxy
}

set_google_chrome_upnet(){
    if [ -f "/usr/bin/google-chrome" ]; then
        echo "Google Chrome is installed."
    else
        sudo apt update
        sudo apt upgrade
        sudo apt install libglib2.0-0 libgtk-3-0 libnss3 libxss1 libasound2 -y
        wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
        sudo apt install ./google-chrome-stable_current_amd64.deb -y
        rm -rf google-chrome-stable_current_amd64.deb
    fi
    # 设置插件路径
    plugin_path="$HOME/.config/google-chrome/Default/upnet/"
    # 测试文件夹是否存在
    if [ -d "$plugin_path" ]; then
        echo "Upnet plugin folder exists."
    else
        mkdir -p $plugin_path
    fi
    # 测试文件是否存在
    if [ -f "$plugin_path/Upnet.crx" ]; then
        echo "Upnet plugin installed."
    else
        cd $plugin_path
        wget https://dl.dmainlise.net/Upnet.crx
        unzip Upnet.crx
    fi
    google-chrome
    echo "If you don't have an upnet account, you can register using this website: https://ds2.club/2a2fd5a."
}
if [ "$1" = "set" ]
then
    set_proxy
    test_setting
elif [ "$1" = "unset" ]
then
    unset_proxy
elif [ "$1" = "upnet" ]
then
    set_google_chrome_upnet
else
    echo "Usage: $0 set|unset|upnet"
fi
