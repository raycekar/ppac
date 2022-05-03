#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

Package *headList = NULL;
unsigned int explicitWordLen = 0;
unsigned int dependsWordLen = 0;
unsigned int explicitWordCt = 0;
unsigned int dependsWordCt = 0;

char *PH = NULL;
char *PHSI = NULL;
char *PHSL = NULL;
char *PHSLQ = NULL;
char *PHQEQ = NULL;
char *PHQQ = NULL;
char *PHQUQ = NULL;
char *PHQI = NULL;
char *PHQ = NULL;
char *PHSY = NULL;
char *PHS = NULL;

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
            explicitWordLen = explicitWordLen + strlen(pkg->pkg);
            explicitWordCt = explicitWordCt + 1;
        }
        else{
            dependsWordLen = dependsWordLen + strlen(pkg->pkg);
            dependsWordCt = dependsWordCt + 1;
        }
        return;
    }
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
    allAvailablePackageList = generateHelper(PHSLQ, allAvailablePackageList);
}

void generateExplicitList()
{
    if (explicitPackageList == NULL)
    {
        explicitPackageList = (char *)malloc(BUFFERSIZE);
        explicitPackageList[0] = '\0';
    }
    explicitPackageList = generateHelper(PHQEQ, explicitPackageList);
}

void generateInstalledList()
{
    if (installedPackageList == NULL)
    {
        installedPackageList = (char *)malloc(BUFFERSIZE);
        installedPackageList[0] = '\0';
    }
    installedPackageList = generateHelper(PHQQ, installedPackageList);
}

void generateOutdatedList()
{
    if (outOfDatePackageList == NULL)
    {
        outOfDatePackageList = (char *)malloc(BUFFERSIZE);
        outOfDatePackageList[0] = '\0';
    }
    outOfDatePackageList = generateHelper(PHQUQ, outOfDatePackageList);
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
    char cmd[BUFFERSIZE] = "";
    strcpy(cmd, PHQQ);
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
        bool cutItShort = FALSE;
    if(strstr(pkgInfo, "#REPO#") == pkgInfo){
        cutItShort = TRUE;
        strcpy(breakpoints[3], breakpoints[7]);
    }
    char *psave1 = NULL, *psave2 = NULL;
    char *tok1 = strtok_r(pkgInfo, "\n", &psave1);
    char *tok2 = NULL;
    char *buf = (char *)malloc(BUFFERSIZE);
    char *colLoc = NULL;
    char depPak[BUFFERSIZE] = "";
    buf[0] = '\0';
    while (pbp < 8)
    {
        if(cutItShort && pbp > 3){
            break;
        }
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
}

char *phQueryInfo(char *pkg)
{
    char cmd[BUFFERSIZE];
    char *concated;
    concated = (char *)malloc(BUFFERSIZE);

    if(isPackageInstalled(pkg)){
        strcpy(cmd, PHQI);
        strcpy(concated, "#LOCAL#");
    }
    else{
        strcpy(cmd, PHSI);
        strcpy(concated, "#REPO#");
    }
    
    strcat(cmd, pkg);
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

void configurationSetup(){
    PH = (char *)malloc(10);
    PH[0] = '\0';
    PHSI = (char *)malloc(20);
    PHSI[0] = '\0';
    PHSLQ = (char *)malloc(20);
    PHSLQ[0] = '\0';
    PHSL = (char *)malloc(20);
    PHSL[0] = '\0';
    PHQI = (char *)malloc(20);
    PHQI[0] = '\0';
    PHQ = (char *)malloc(20); 
    PHQ[0] = '\0';
    PHSY = (char *)malloc(20);
    PHSY[0] = '\0';
    PHS = (char *)malloc(20); 
    PHS[0] = '\0';
    PHQEQ = (char *)malloc(20); 
    PHQEQ[0] = '\0';
    PHQQ = (char *)malloc(20); 
    PHQQ[0] = '\0';
    PHQUQ = (char *)malloc(20); 
    PHQUQ[0] = '\0';

    FILE *p;
    p = popen("echo ~", "r");
    char path[BUFFERSIZE] = "";
    fgets(path, BUFFERSIZE, p);
    trimString(path);
    pclose(p);
    strcat(path, "/.config/ppac/ppac.conf");
    FILE *conf = fopen(path, "r");
    if(conf == NULL){
        strcpy(PH, "pacman");
    }
    else{
        char *buf = (char *)malloc(BUFFERSIZE);
        char *pBuf = fgets(buf, BUFFERSIZE, conf);
        char key[20] = "";
        char value[20] = "";
        
        while(pBuf != NULL){
            char *eloc = strstr(buf, "=");
            if(eloc != NULL){
                strcpy(value,eloc + 1);
                eloc[0] = '\0';
                strcpy(key, buf);
                trimString(key);
                trimString(value);
                if(strcmp(key, "PKGHELPER") == 0){
                    strcpy(PH, value);
                }
            }
            pBuf = fgets(buf, BUFFERSIZE, conf);
        }
        fclose(conf);
    }
    strcpy(PHSI, PH);
    strcat(PHSI, " -Si ");
    strcpy(PHSLQ, PH);
    strcat(PHSLQ, " -Slq ");
    strcpy(PHSL, PH);
    strcat(PHSL, " -Sl ");
    strcpy(PHQI, PH);
    strcat(PHQI, " -Qi ");
    strcpy(PHQ, PH);
    strcat(PHQ, " -Q ");
    strcpy(PHSY, PH);
    strcat(PHSY, " -Sy ");
    strcpy(PHS, PH);
    strcat(PHS, " -S ");
    strcpy(PHQEQ, PH);
    strcat(PHQEQ, " -Qeq ");
    strcpy(PHQQ, PH);
    strcat(PHQQ, " -Qq ");
    strcpy(PHQUQ, PH);
    strcat(PHQUQ, " -Quq ");
}