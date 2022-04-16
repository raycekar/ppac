#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

Package *createPackage(char *pkg, bool asdep){
    Package *pak = (Package *)malloc(sizeof(Package));
    strcpy(pak->pkg, pkg);
    pak->asdep = asdep;
    pak->next = NULL;
    return pak;
}

Package *getDependsOn(char *pkg)
{
    char *doStr = pacmanOutputHelper(PHQI, pkg, DEPSONQ, DEPSON, OPTDEPS);
    strtok(doStr," ");
    return createPackage(pkg, TRUE);
}

// void getPackageDependencies(char *pkg, Package *pkgListHead)
// {
//     char cmd[100] = PHSI;
//     strcat(cmd, pkg);
//     char output[2000] = "";
//     char moreOutput[1000] = "";
//     FILE *p;
//     p = popen(cmd, "r");
//     char *ifFound = NULL;
//     while (fgets(output, 2000, p) != NULL)
//     {
//         ifFound = strstr(output, "Depends On");
//         if (ifFound != NULL)
//         {
//             while(fgets(moreOutput, 1000, "Optional Deps") == NULL){
//                 strcat(output, moreOutput);
//             }
//             ifFound = strstr(output, ":") + 1;
//             while (*ifFound == ' ')
//             {
//                 ifFound = ifFound + 1;
//             }
//             break;
//         }
//     }
//     if (strcmp(ifFound, "None\n") != 0)
//     {
//         char *tok = trimString(strtok(ifFound, " "));
//         do
//         {
//             if (isValidPackage(tok))
//             {
//                 Package *link = NULL;
//                 link = (Package *)malloc(sizeof(Package));
//                 strcpy(link->pkg, tok);
//                 if (isPackageInstalled(tok))
//                 {
//                     if (isPackageExplicit(tok))
//                     {
//                         link->asdep = FALSE;
//                     }
//                     else
//                     {
//                         link->asdep = TRUE;
//                     }
//                 }
//                 else
//                 {
//                     link->asdep = FALSE;
//                 }
//                 link->next = NULL;
//                 pkgListHead->next = link;
//             }
//             tok = strtok(NULL, " ");
//             if(tok != NULL){
//                 tok = trimString(tok);
//             }
//         } while (tok != NULL);
//     }
// }

bool isPackageExplicit(char *pkg)
{
    if (isPackageInstalled(pkg) == FALSE)
    {
        return FALSE;
    }
    char ir[] = "\"Install Reason\"";
    char cmd[strlen(PHQI) + strlen(pkg) + strlen(GREP) + strlen(ir) + TB];
    strcpy(cmd, PHQI);
    strcat(cmd, pkg);
    strcat(cmd, GREP);
    strcat(cmd, ir);
    char output[BUFFERSIZE] = "";
    FILE *p;
    p = popen(cmd, "r");
    fgets(output, BUFFERSIZE, p);
    pclose(p);
    if (strstr(output, "Explicitly installed") != NULL)
    {
        return TRUE;
    }
    return FALSE;
}

bool isPackageInstalled(char *pkg)
{
    char cmd[strlen(PHQ) + strlen(pkg) + TB];
    strcpy(cmd, PHQ);
    strcat(cmd, pkg);
    char output[BUFFERSIZE] = "";
    FILE *p;
    p = popen(cmd, "r");
    fgets(output, 100, p);
    pclose(p);
    if (strstr(output, pkg) != NULL)
    {
        return TRUE;
    }
    return FALSE;
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

char *pacmanOutputHelper(char *ph, char *pkg, char *patternQ, char *pattern, char *pttrnExclude){
    char cmd[strlen(ph) + strlen(pkg) + strlen(GREP) + strlen(patternQ) + TB];
    strcpy(cmd, ph);
    strcat(cmd, pkg);
    strcat(cmd, GREP);
    strcat(cmd, patternQ);
    char *cmdd = cmd;
    char concated[BUFFERSIZE] = "";
    char tmp[BUFFERSIZE] = "";
    FILE *p;
    p = popen(cmd, "r");
    char *pout = fgets(tmp, BUFFERSIZE, p);
    while(pout != NULL)
    {  
        if(strstr(tmp, pttrnExclude) != NULL){
            break;
        }
        strcat(concated, tmp);
        pout = fgets(tmp, BUFFERSIZE, p);
    }
    pclose(p);
    if (strstr(concated, pattern) == NULL)
    {
        return NULL;
    }
    strcpy(concated, strstr(concated, ":") + 1);
    char *output = (char *)malloc(strlen(concated));
    strcpy(output, concated);
    return output;
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
    memmove(str, pstr, strlen(pstr));
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

void updateDatabase()
{
    FILE *p;
    p = popen(PHSYY, "r");
    pclose(p);
}