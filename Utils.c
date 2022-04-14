#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

void getPackageDependencies(char *pkg, Package *pkgListHead)
{
    char cmd[100] = "pacman -Si ";
    strcat(cmd, pkg);
    char output[2000] = "";
    FILE *p;
    p = popen(cmd, "r");
    char *ifFound = NULL;
    while (fgets(output, 2000, p) != NULL)
    {
        ifFound = strstr(output, "Depends On");
        if (ifFound != NULL)
        {
            ifFound = strstr(output, ":") + 1;
            while (*ifFound == ' ')
            {
                ifFound = ifFound + 1;
            }
            break;
        }
    }
    if (strcmp(ifFound, "None\n") != 0)
    {
        char *tok = trimString(strtok(ifFound, " "));
        do
        {
            if (isValidPackage(tok))
            {
                Package *link = NULL;
                link = (Package *)malloc(sizeof(Package));
                strcpy(link->pkg, tok);
                if (isPackageInstalled(tok))
                {
                    if (isPackageExplicit(tok))
                    {
                        link->asdep = FALSE;
                    }
                    else
                    {
                        link->asdep = TRUE;
                    }
                }
                else
                {
                    link->asdep = FALSE;
                }
                link->next = NULL;
                pkgListHead->next = link;
            }
            tok = strtok(NULL, " ");
            if(tok != NULL){
                tok = trimString(tok);
            }
        } while (tok != NULL);
    }
}

bool isPackageExplicit(char *pkg)
{
    if (isPackageInstalled(pkg) == FALSE)
    {
        return FALSE;
    }
    char cmd[100] = "pacman -Qi ";
    strcat(cmd, pkg);
    char output[2000] = "";
    FILE *p;
    p = popen(cmd, "r");
    char *ifFound = NULL;
    while (fgets(output, 2000, p) != NULL)
    {
        ifFound = strstr(output, "Install Reason");
        if (ifFound != NULL)
        {
            if (strstr(output, "Explicitly installed") != NULL)
            {
                return TRUE;
            }
            break;
        }
    }
    pclose(p);
    return FALSE;
}

bool isPackageInstalled(char *pkg)
{
    char cmd[50] = "pacman -Qi ";
    strcat(cmd, pkg);
    char output[100] = "";
    FILE *p;
    p = popen(cmd, "r");
    fgets(output, 100, p);
    pclose(p);
    if (
        output[0] == 'N' &&
        output[1] == 'a' &&
        output[2] == 'm' &&
        output[3] == 'e')
    {
        return TRUE;
    }
    return FALSE;
}

bool isValidPackage(char *str)
{
    if(strlen(str) == 0){
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

char *trimString(char *str)
{
    const char vrr[3] = {' ', '\n'};
    int i;
    int j;
    bool ftrimmed = FALSE;
    for (i = 0; i < strlen(str); i++)
    {
        if (ftrimmed)
        {
            break;
        }
        for (j = 0; j < strlen(vrr); j++)
        {
            if (str[i] == vrr[j])
            {
                str = str + 1;
                i--;
                break;
            }
            if (j == strlen(vrr) - 1)
            {
                ftrimmed = TRUE;
            }
        }
    }
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
