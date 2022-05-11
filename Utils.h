
#define bool char
#define TRUE 1
#define FALSE 0
#define BUFFERSIZE 5000

// Package Helper (PH)
extern char *PH;
extern char *PHSI;
extern char *PHSL; 
extern char *PHSLQ;
extern char *PHQEQ;
extern char *PHQQ;
extern char *PHQUQ;
extern char *PHQI; 
extern char *PHQ; 
extern char *PHSY; 
extern char *PHS;
extern char *PHSYU; 

typedef struct Package
{
   char *pkg;  // package name
   bool isExplicit;
   struct Package *next;
} Package;

extern Package *headList;
extern unsigned int explicitWordLen;
extern unsigned int dependsWordLen;
extern unsigned int explicitWordCt;
extern unsigned int dependsWordCt;

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
void configurationSetup();
void fullUpgrade();


