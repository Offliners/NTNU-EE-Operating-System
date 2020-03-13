# Week 1 Note
## Build Environment
1. install VirtualBox(https://www.virtualbox.org/)

2. Download Ubuntu(http://free.nchc.org.tw/ubuntu-cd/bionic/)
    - ubuntu-18.04.4-desktop-amd64.iso

3. Use Ubuntu on VirtualBox

## Write a Program with VIM
* 安裝vim : `sudo apt install vim`
* 安裝gcc : `sudo apt install gcc`
* 建立子目錄lab1 : `mkdir lab1`
* 進入子目錄 : `cd lab1`
* 啟動vim編輯test.c : `vim test.c`
* 查看目錄內容 : `ls`
* 編譯test.c : `gcc test.c -o test`
* 執行test : `./test`
* 編譯test.c : `gcc -Wall –g –o test test.c`
* 啟動gdb : `gdb -q test`
