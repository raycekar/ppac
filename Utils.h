
#define bool char
#define TRUE 1
#define FALSE 0

typedef struct Package
{
   char pkg[50]; // package name
   bool asdep;    // As dependency
   struct Package *next;
} Package;

void getPackageDependencies(char *pkg, Package *pkgListHead);
bool isPackageExplicit(char *pkg);
bool isPackageInstalled(char *pkg);
bool isValidPackage(char *str);
char *trimString(char *str);