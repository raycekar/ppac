#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "Utils.h"

void addUniquePackages(Package *head, Package *newPkgList)
{
   if(newPkgList == NULL){
      return;
   }
   Package *curNewPkg = newPkgList;
   Package *newNext = newPkgList->next;
   while (curNewPkg != NULL)
   {
      Package *pointer = head;
      Package *prevPointer = NULL;
      while (pointer != NULL && strcmp(pointer->pkg, curNewPkg->pkg) != 0)
      {
         prevPointer = pointer;
         pointer = pointer->next;
      }
      if (pointer == NULL)
      {
         prevPointer->next = curNewPkg;
      }
      curNewPkg->next = NULL;
      if(pointer != NULL){
         destroyPackage(curNewPkg);
      }
      curNewPkg = newNext;
      if(curNewPkg != NULL){
         newNext = curNewPkg->next;
      }
   }
}

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      printf("Error: ppac only accepts one agument.\n");
      return 1;
   }
   // signal(SIGPIPE, SIG_IGN);
   printf("Syncing database.\n");
   updateDatabase();
   char *pkg = argv[1];
   printf("Checking package.\n");
   if (isPackageInstalled(pkg) && isOutdated(pkg) == FALSE)
   {
      printf("\n%s is already installed and up-to-date.\nDoing nothing.\n\n", pkg);
      return 1;
   }

   Package *head = createPackage(pkg, !(isPackageExplicit(trimString(pkg))));
   Package *whereAmI = head;

   Package *tempPkgList = NULL;
   int asDepsSize = 0;
   int notAsDepsSize = 0;
   while (whereAmI != NULL)
   {
      if(strcmp(whereAmI->pkg, "tzdata") == 0){
         printf("Hi\n");
      }
      printf("%s\n", whereAmI->pkg);
      tempPkgList = getDependsOn(whereAmI->pkg);
      addUniquePackages(head, tempPkgList);
      tempPkgList = getOptionalDepsInstalled(whereAmI->pkg);
      addUniquePackages(head, tempPkgList);
      tempPkgList = getRequiredBy(whereAmI->pkg);
      addUniquePackages(head, tempPkgList);
      tempPkgList = getOptionalFor(whereAmI->pkg);
      addUniquePackages(head, tempPkgList);
      if(whereAmI->asdep){
         asDepsSize = asDepsSize + strlen(whereAmI->pkg);
      }
      else{
         notAsDepsSize = notAsDepsSize + strlen(whereAmI->pkg);
      }
      whereAmI = whereAmI->next;
   }

   int buff = 1000;
   asDepsSize = asDepsSize + buff;
   notAsDepsSize = notAsDepsSize + buff;

   char *adcmd = (char *)malloc(asDepsSize);
   char *nadcmd = (char *)malloc(notAsDepsSize);
   strcpy(adcmd, "");
   strcpy(nadcmd, "");

   whereAmI = head;
   while (whereAmI != NULL)
   {
      if(whereAmI->asdep){
         strcat(adcmd, whereAmI->pkg);
      }
      else{
         strcat(nadcmd, whereAmI->pkg);
      }
   }
   

   return 0;
}