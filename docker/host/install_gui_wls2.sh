#/usr/bin/env bash
# https://learn.microsoft.com/zh-cn/windows/wsl/tutorials/gui-apps
sudo apt install x11-apps -y
sudo apt install xvfb
Xvfb :1 -screen 0 1024x768x16 &
export DISPLAY=:0
# xcalc, xclock, xeyes