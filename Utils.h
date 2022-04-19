
#define bool char
#define TRUE 1
#define FALSE 0
#define BUFFERSIZE 5000

// Package Helper
#define PHSI "yay -Si "
#define PHSL "yay -Sl "
#define PHQI "yay -Qi "
#define PHQ "yay -Q "
#define PHSY "yay -Sy "

#define GREP " | grep -A 100 --no-group-separator "

// Defined Pacman Output
#define NAME "Name"
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
#define INSTALLSCRIPT "Install Script"
#define CONFLICTSWITH "Conflicts With"

const int TB = 10;

typedef struct Package
{
   char *pkg;  // package name
   bool asdep; // As dependency
   struct Package *next;
} Package;

Package *createPackage(char *pkg, bool asdep);
void destroyPackage(Package *pak);
char *getCorrectPackageName(char *pkg);
Package *getDependsOn(char *pkg);
Package *getOptionalDepsInstalled(char *pkg);
Package *getRequiredBy(char *pkg);
Package *getOptionalFor(char *pkg);
void getPackageDependencies(char *pkg, Package *pkgListHead);
bool isOutdated(char *pkg);
bool isPackageExplicit(char *pkg);
bool isPackageInstalled(char *pkg);
bool isValidPackage(char *str);
char *pacmanOutputHelper(char *ph, char *pkg, char *patternQ, char *pattern, char *pttrnExclude);
char *trimString(char *str);
void updateDatabase();