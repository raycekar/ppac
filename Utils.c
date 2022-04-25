#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

Package *headList = NULL;
int Explicit = 0;
int Depends = 0;

char *installedPackageList;
char *outOfDatePackageList;
char *allAvailablePackageList;
char *explicitPackageList;

void addPackageToList(Package *pkg);
void destroyPackage(Package *pak);
void genearateAllPackageList();
void generateExplicitList();
void generateInstalledList();
void generateOutdatedList();
char *generateHelper(char *cmd, char *storedLocation);
char *getCorrectName(char *pkg);
bool searchHelper(char *pkg, char *storedLocation);
char *phQueryInfo(char *pkg);


void addPackageToList(Package *pkg){
    Package *pointer = headList;
    while(pointer->next != NULL && strcmp(pointer->pkg, pkg->pkg) != 0){
        pointer = pointer->next;
    }
    if(strcmp(pointer->pkg, pkg->pkg) != 0){
        pointer->next = pkg;
        if(pkg->isExplicit){
            Explicit = Explicit + strlen(pkg->pkg);
        }
        else{
            Depends = Depends + strlen(pkg->pkg);
        }
        return;
    }
    printf("%s already in headlist.\n", pkg->pkg);
    destroyPackage(pkg);
}

Package *createPackage(char *pkg, bool isExplicit)
{
    Package *pak = (Package *)malloc(sizeof(Package));
    pak->pkg = (char *)malloc(strlen(pkg) + 1);
    strcpy(pak->pkg, pkg);
    pak->isExplicit = isExplicit;
    pak->next = NULL;
    return pak;
}

void destroyPackage(Package *pak)
{
    free(pak->pkg);
    free(pak);
}

void initilizeAll()
{
    genearateAllPackageList();
    generateInstalledList();
    generateOutdatedList();
    generateExplicitList();
}

void genearateAllPackageList()
{
    if (allAvailablePackageList == NULL)
    {
        allAvailablePackageList = (char *)malloc(BUFFERSIZE);
        allAvailablePackageList[0] = '\0';
    }
    allAvailablePackageList = generateHelper("yay -Slq", allAvailablePackageList);
}

void generateExplicitList()
{
    if (explicitPackageList == NULL)
    {
        explicitPackageList = (char *)malloc(BUFFERSIZE);
        explicitPackageList[0] = '\0';
    }
    explicitPackageList = generateHelper("yay -Qeq", explicitPackageList);
}

void generateInstalledList()
{
    if (installedPackageList == NULL)
    {
        installedPackageList = (char *)malloc(BUFFERSIZE);
        installedPackageList[0] = '\0';
    }
    installedPackageList = generateHelper("yay -Qq", installedPackageList);
}

void generateOutdatedList()
{
    if (outOfDatePackageList == NULL)
    {
        outOfDatePackageList = (char *)malloc(BUFFERSIZE);
        outOfDatePackageList[0] = '\0';
    }
    outOfDatePackageList = generateHelper("yay -Quq", outOfDatePackageList);
}

char *generateHelper(char *cmd, char *storedLocation)
{
    char output[BUFFERSIZE] = "";
    int dataSize = BUFFERSIZE;
    FILE *p = popen(cmd, "r");
    char *pout = fgets(output, BUFFERSIZE, p);
    char *temp = NULL;
    while (pout != NULL)
    {
        if ((strlen(output) + strlen(storedLocation) + 1) / dataSize > 0)
        {
            dataSize = BUFFERSIZE * (((strlen(output) + strlen(storedLocation) + 1) / BUFFERSIZE) + 1);
            temp = (char *)malloc(dataSize);
            memmove(temp, storedLocation, strlen(storedLocation) + 1);
            free(storedLocation);
            storedLocation = temp;
        }
        strcat(storedLocation, output);
        pout = fgets(output, BUFFERSIZE, p);
    }
    pclose(p);
    return storedLocation;
}

char *getCorrectName(char *pkg)
{
    char cmd[BUFFERSIZE] = "yay -Qq ";
    strcat(cmd, pkg);
    FILE *p;
    p = popen(cmd, "r");
    char buf[BUFFERSIZE] = "";
    char *pout = fgets(buf, BUFFERSIZE, p);
    trimString(pout);
    pclose(p);
    return (char *)malloc(strlen(pout));
}

bool isExplicit(char *pkg)
{
    return searchHelper(pkg, explicitPackageList);
}

bool isOutdated(char *pkg)
{
    return searchHelper(pkg, outOfDatePackageList);
}

bool isPackageInstalled(char *pkg)
{
    return searchHelper(pkg, installedPackageList);
}

bool isInDatabase(char *pkg)
{
    return searchHelper(pkg, allAvailablePackageList);
}

bool searchHelper(char *pkg, char *storedLocation)
{
    
    char *temp;
    temp = (char *)malloc(strlen(storedLocation)+1);
    memmove(temp, storedLocation, strlen(storedLocation) + 1);
    char *ptok;
    char *tok = strtok_r(temp, "\n", &ptok);
    while (tok != NULL)
    {
        if (strcmp(pkg, tok) == 0)
        {
            break;
        }
        tok = strtok_r(NULL, "\n", &ptok);
    }
    free(temp);
    if (tok == NULL)
    {
        return FALSE;
    }
    return TRUE;
}

char *trimString(char *str)
{
    char *pstr = str;
    const char vrr[3] = {' ', '\n'};
    int i;
    int j;
    bool ftrimmed = FALSE;
    for (i = 0; i < strlen(pstr); i++)
    {
        if (ftrimmed)
        {
            break;
        }
        for (j = 0; j < strlen(vrr); j++)
        {
            if (pstr[i] == vrr[j])
            {
                pstr = pstr + 1;
                i--;
                break;
            }
            if (j == strlen(vrr) - 1)
            {
                ftrimmed = TRUE;
            }
        }
    }
    memmove(str, pstr, strlen(pstr) + 1);
    bool btrimmed = FALSE;
    for (i = strlen(str) - 1; i >= 0; i--)
    {
        if (btrimmed)
        {
            break;
        }
        for (j = 0; j < strlen(vrr); j++)
        {
            if (str[i] == vrr[j])
            {
                str[i] = '\0';
                break;
            }
            if (j == strlen(vrr) - 1)
            {
                btrimmed = TRUE;
            }
        }
    }
    return str;
}

void syncDatabase()
{
    system(PHSY);
}

void updateDependencyPackages(char *pkg)
{
    char *pkgInfo = phQueryInfo(pkg);
    int pbp = 0;
    char breakpoints[][20] = {
        "Depends On", "Optional Deps",
        "Optional Deps", "Required By",
        "Required By", "Optional For",
        "Optional For", "Conflicts With"
    };
    char *psave1 = NULL, *psave2 = NULL;
    char *tok1 = strtok_r(pkgInfo, "\n", &psave1);
    char *tok2 = NULL;
    char *buf = (char *)malloc(BUFFERSIZE);
    char *colLoc = NULL;
    char depPak[500] = "";
    buf[0] = '\0';
    while (pbp < (sizeof(breakpoints)/sizeof(breakpoints[0])))
    {
        if (strstr(tok1, breakpoints[pbp]) == tok1)
        {
            colLoc = strstr(tok1, ":");
            strcat(buf, colLoc + 1);
            strcat(buf, "\n");
            tok1 = strtok_r(NULL, "\n", &psave1);
            while (strstr(tok1, breakpoints[pbp + 1]) != tok1)
            {
                strcat(buf, tok1);
                strcat(buf, "\n");
                tok1 = strtok_r(NULL, "\n", &psave1);
            }
            if(strcmp(trimString(buf), "None") == 0){
                buf[0] = '\0';
                pbp = pbp + 2;
                continue;
            }
            if (pbp == 0 || pbp == 4 || pbp == 6)
            {
                depPak[0] = '\0';
                tok2 = strtok_r(buf, " ", &psave2);
                do{
                    strcpy(depPak,tok2);
                    trimString(depPak);
                    if(isPackageInstalled(depPak) && isOutdated(depPak)){
                        addPackageToList(createPackage(depPak,isExplicit(depPak)));
                    }
                    else if(isPackageInstalled(depPak) && !isOutdated(depPak)){
                        tok2 = strtok_r(NULL, " ", &psave2);
                        continue;
                    }
                    else if(isInDatabase(depPak)){
                        tok2 = strtok_r(NULL, " ", &psave2);
                        continue;
                    }
                    else if(isValidPackage(depPak))
                    {
                        char *newName = getCorrectName(depPak);
                        if(isPackageInstalled(newName) && isOutdated(newName)){
                            addPackageToList(createPackage(newName,isExplicit(newName)));
                        }
                        free(newName);
                    }
                    tok2 = strtok_r(NULL, " ", &psave2);
                } while(tok2 != NULL);
            }
            else if (pbp == 2)
            {
                depPak[0] = '\0';
                tok2 = strtok_r(buf, "\n", &psave2);
                do{
                    char *insloc = strstr(tok2, "[installed]");
                    if(insloc != NULL){
                        strcpy(depPak, tok2);
                        colLoc = strstr(depPak, ":");
                        if(colLoc == NULL){
                            insloc[0] = '\0';
                        }
                        trimString(depPak);
                        if(isOutdated(depPak)){
                            addPackageToList(createPackage(depPak,isExplicit(depPak)));
                        }
                    }
                    tok2 = strtok_r(NULL, "\n",  &psave2);
                }while(tok2 != NULL);
                
            }
            
            buf[0] = '\0';
            pbp = pbp + 2;
        }
        else{
            tok1 = strtok_r(NULL, "\n", &psave1);
        }
    }
    free(buf);
    free(pkgInfo);
    printf("Im done.\n");
}

char *phQueryInfo(char *pkg)
{
    char cmd[strlen(PHQI) + strlen(pkg) + 1];
    strcpy(cmd, PHQI);
    strcat(cmd, pkg);

    char *concated;
    concated = (char *)malloc(BUFFERSIZE);
    strcpy(concated, "");
    int conSize = BUFFERSIZE;
    char tmp[BUFFERSIZE] = "";
    FILE *p;
    p = popen(cmd, "r");
    char *pout = fgets(tmp, BUFFERSIZE, p);
    while (pout != NULL)
    {
        if ((strlen(concated) + strlen(tmp) + 1) / conSize > 0)
        {
            conSize = BUFFERSIZE * (((strlen(concated) + strlen(tmp) + 1) / conSize) + 1);
            char *deleteMe;
            deleteMe = (char *)malloc(conSize);
            memmove(deleteMe, concated, strlen(concated));
            free(concated);
            concated = deleteMe;
        }
        strcat(concated, tmp);
        pout = fgets(tmp, BUFFERSIZE, p);
    }
    pclose(p);
    return concated;
}

bool isValidPackage(char *str)
{
    if (strlen(str) == 0)
    {
        return FALSE;
    }
    const char validChars[] = "abcdefghijklmnopqrstuvwxyz@._+-";
    int i;
    int j;
    bool lttrPass = FALSE;
    for (i = 0; i < strlen(str); i++)
    {
        for (j = 0; j < strlen(validChars); j++)
        {
            if (str[i] == validChars[j])
            {
                lttrPass = TRUE;
                break;
            }
        }
        if (lttrPass == FALSE)
        {
            return FALSE;
        }
        lttrPass = FALSE;
    }
    return TRUE;
}
