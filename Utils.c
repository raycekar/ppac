#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

Package *createPackage(char *pkg, bool asdep)
{
    Package *pak = (Package *)malloc(sizeof(Package));
    pkg = getCorrectPackageName(pkg);
    pak->pkg = (char *)malloc(sizeof(pkg) + 1);
    strcpy(pak->pkg, pkg);
    pak->asdep = asdep;
    pak->next = NULL;
    return pak;
}

void destroyPackage(Package *pak)
{
    free(pak->pkg);
    free(pak);
}

char *getCorrectPackageName(char *pkg)
{
    if (isPackageInstalled(pkg))
    {
        return trimString(pacmanOutputHelper(PHQI, pkg, NAME, NAME, VER));
    }
    return pkg;
}

Package *getDependsOn(char *pkg)
{
    char *dpon = pacmanOutputHelper(PHQI, pkg, DEPSONQ, DEPSON, OPTDEPS);
    char *tok = strtok(dpon, " ");
    Package *head = NULL;
    Package *tail = NULL;
    while (tok != NULL)
    {
        bool skip = FALSE;
        trimString(tok);
        if (!(isPackageInstalled(tok)) || !(isOutdated(tok)))
        {
            skip = TRUE;
        }

        if (!skip && head == NULL)
        {
            head = createPackage(tok, !(isPackageExplicit(tok)));
            tail = head;
        }
        else if (!skip)
        {
            tail->next = createPackage(tok, !(isPackageExplicit(tok)));
            tail = tail->next;
        }
        tok = strtok(NULL, " ");
    }
    free(dpon);
    return head;
}

Package *getOptionalDepsInstalled(char *pkg)
{
    char *optDeps = pacmanOutputHelper(PHQI, pkg, OPTDEPSQ, OPTDEPS, REQBY);
    char *tok = strtok(optDeps, "\n");
    Package *head = NULL;
    Package *tail = NULL;
    char installedKey[] = "[installed]";
    do
    {
        bool skip = FALSE;
        trimString(tok);
        if (strstr(tok, installedKey) != NULL)
        {
            strstr(tok, ":")[0] = '\n';
            if (!(isPackageInstalled(tok)) || !(isOutdated(tok)))
            {
                skip = TRUE;
            }

            if (!skip && head == NULL)
            {
                head = createPackage(tok, !(isPackageExplicit(tok)));
                tail = head;
            }
            else if (!skip)
            {
                tail->next = createPackage(tok, !(isPackageExplicit(tok)));
                tail = tail->next;
            }
        }
        tok = strtok(NULL, "\n");
    } while (tok != NULL);
    free(optDeps);
    return head;
}

bool isOutdated(char *pkg)
{
    char *local_pkg = pkg;
    char dx[] = "Description";
    char *databaseV = pacmanOutputHelper(PHSI, local_pkg, VER, VER, dx);
    if (databaseV == NULL)
    {
        local_pkg = getCorrectPackageName(local_pkg);
        databaseV = pacmanOutputHelper(PHSI, local_pkg, VER, VER, dx);
    }
    char *localV = pacmanOutputHelper(PHQI, local_pkg, VER, VER, dx);
    if (pkg != local_pkg)
    {
        free(local_pkg);
    }
    trimString(databaseV);
    trimString(localV);
    int result = strcmp(localV, databaseV);
    free(databaseV);
    free(localV);
    if (result == 0)
    {
        return FALSE;
    }
    return TRUE;
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
    char *expStr = pacmanOutputHelper(PHQI, pkg, INSTALLREASONQ, INSTALLREASON, INSTALLSCRIPT);
    char *result = strstr(expStr, "Explicitly installed");
    free(expStr);
    if (result != NULL)
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
    fgets(output, BUFFERSIZE, p);
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

char *pacmanOutputHelper(char *ph, char *pkg, char *patternQ, char *pattern, char *pttrnExclude)
{
    char redirect[] = " 2>/dev/null";
    char cmd[strlen(ph) + strlen(pkg) + strlen(GREP) + strlen(patternQ) + strlen(redirect) + TB];
    strcpy(cmd, ph);
    strcat(cmd, pkg);
    strcat(cmd, GREP);
    strcat(cmd, patternQ);
    strcat(cmd, redirect);
    // char *cmdd = cmd;
    char concated[BUFFERSIZE] = "";
    char tmp[BUFFERSIZE] = "";
    FILE *p;
    p = popen(cmd, "r");
    char *pout = fgets(tmp, BUFFERSIZE, p);
    while (pout != NULL)
    {
        if (strstr(tmp, pttrnExclude) != NULL)
        {
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
    char *colLoc = strstr(concated, ":") + 1;
    memmove(concated, colLoc, strlen(colLoc));
    char *output = (char *)malloc(strlen(concated) + 1);
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

void updateDatabase()
{
    system(PHSY);
}