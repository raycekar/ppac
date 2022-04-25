
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

// Defined Pacman Output
#define NAME "Name"
#define VER "Version"
#define DEPSON "Depends On"
#define OPTDEPS "Optional Deps"
#define REQBY "Required By"
#define OPTFOR "Optional For"
#define CONFLICTSWITH "Conflicts With"

typedef struct Package
{
   char *pkg;  // package name
   bool isExplicit;
   struct Package *next;
} Package;

extern Package *headList;
extern int Explicit;
extern int Depends;

void initilizeAll();
bool isExplicit(char *pkg);
bool isOutdated(char *pkg);
bool isPackageInstalled(char *pkg);
bool isInDatabase(char *pkg);
char *trimString(char *str);
void syncDatabase();
void updateDependencyPackages(char *pkg);
bool isValidPackage(char *str);
Package *createPackage(char *pkg, bool isExplicit);


