# ppac (Partial Pacman)
Partial Pacman package updater/installer.

# Overview (In Development):

ppac will be used for updating and installing new packages and it's dependencies without having to update every package.  The idea is the tool will scrape pacman for a given package for it's dependencies and installed optional dependencies (if package is to be upgraded) recursively updating only the necessary.  It is stated in many places to update everything every time, but when only wanting to install a single package shouldn't result in a broken system nor having to install a gb or more.

# Goals (subject to change):

1. interface with only pacman as a dependency (yay or paru could potentially come later down the line for AUR interaction)
2. update the specified package along with dependencies and only installed optional dependencies
