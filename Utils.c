#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

Package *getHelper(char *PH, char *pkg, char *patternQ, char *pattern, char *pttrnExclude);

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
    Package *head = getHelper(PHQI, pkg, DEPSONQ, DEPSON, OPTDEPS);
    return head;
}

Package *getOptionalDepsInstalled(char *pkg)
{
    char *optDeps = pacmanOutputHelper(PHQI, pkg, OPTDEPSQ, OPTDEPS, REQBY);
    char *tok = strtok(optDeps, "\n");
    Package *head = NULL;
    Package *tail = NULL;
    char installedKey[] = "[installed]";
    while (tok != NULL)
    {
        bool skip = FALSE;
        trimString(tok);
        if (strstr(tok, installedKey) != NULL)
        {
            strstr(tok, ":")[0] = '\0';
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
    }
    free(optDeps);
    return head;
}

Package *getOptionalFor(char *pkg){
    Package *head = getHelper(PHQI, pkg, OPTFORQ, OPTFOR, CONFLICTSWITH);
    return head;
}

Package *getRequiredBy(char *pkg){
    Package *head = getHelper(PHQI, pkg, REQBYQ, REQBY, OPTFOR);
    return head;
}
Package *getHelper(char *PH, char *pkg, char *patternQ, char *pattern, char *pttrnExclude){
    char *dpon = pacmanOutputHelper(PH, pkg, patternQ, pattern, pttrnExclude);
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
    strcat(cmd, redirect);
    strcat(cmd, GREP);
    strcat(cmd, patternQ);

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
    memmove(concated, colLoc, strlen(colLoc) + 1);
    if (strstr(concated, " None\n") != NULL)
    {
        return NULL;
    }
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