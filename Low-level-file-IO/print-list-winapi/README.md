# for MANJARO-LINUX(OR ARCH)

## Install wine
    sudo pacman -S wine winetricks wine-mono wine_gecko
## Install mingw
    sudo pacman -S mingw-w64
## Compiling
    i686-w64-mingw32-gcc program.c
## Run with wine
    WINEDEBUG=-all wine a.exe