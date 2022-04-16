
#define bool char
#define TRUE 1
#define FALSE 0
#define BUFFERSIZE 5000

// Package Helper
#define PHSI "yay -Si "
#define PHQI "yay -Qi "
#define PHQ "yay -Q "
#define PHSYY "yay -Syy "

#define GREP " | grep -A 100 --no-group-separator "

// Defined Pacman Output
#define VER "Version"
#define DEPSON "Depends On"
#define DEPSONQ "\"Depends On\""
#define OPTDEPS "Optional Deps"
#define OPTDEPSQ "\"Optional Deps\""
#define REQBY "Required By"
#define REQBYQ "\"Required By\""
#define OPTFOR "Optional For"
#define OPTFORQ "\"Optional For\""
#define INSTALLREASON "Install Reason"
#define INSTALLREASONQ "\"Install Reason\""

const int TB = 10;

typedef struct Package
{
   char *pkg;  // package name
   bool asdep; // As dependency
   struct Package *next;
} Package;

Package *createPackage(char *pkg, bool asdep);
Package *getDependsOn(char *pkg);
void getPackageDependencies(char *pkg, Package *pkgListHead);
bool isPackageExplicit(char *pkg);
bool isPackageInstalled(char *pkg);
bool isValidPackage(char *str);
char *pacmanOutputHelper(char *ph, char *pkg, char *patternQ, char *pattern, char *pttrnExclude);
char *trimString(char *str);