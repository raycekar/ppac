# ppac (Partial Pacman)
Partial Pacman package updater/installer.

# Overview (In Development):

ppac will be used for updating and/or installing new packages and it's dependencies without having to update every package.  The idea is the tool will scrape the output of a package helper (pacman/yay/paru) to determine which packages need to be updated for a given package (also updating installed optional dependencies). It is stated in many places to update everything every time, but when only wanting to install a single package shouldn't result in a broken system nor having to install a gb or more.  So in short, use at your own risk :)

# AUR Installation
Using your package helper of choice, for example: yay -S ppac-git.
I suggest setting up the config file if you do use a package helper.

# Manual Installation:

You will need gcc for compiling.

1) git clone https://github.com/raycekar/ppac.git
2) cd ppac
3) gcc -g \*.c -o ppac
4) you can now either leave and use it in this location or move it to /usr/bin directory

# Usage
ppac \<package name\>

# Configuration

ppac will default to using pacman, but you could change it to yay/paru by creating a config file ~/.config/ppac/ppac.conf and paste the line "PKGHELPER=yay".
