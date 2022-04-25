#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "Utils.h"

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      printf("Error: ppac only accepts one agument.\n");
      return 1;
   }
   // signal(SIGPIPE, SIG_IGN);
   //syncDatabase();
   initilizeAll();
   char *pkg = (char *)malloc(strlen(argv[1]));
   strcpy(pkg, argv[1]);
   printf("Checking package.\n");
   // if (isPackageInstalled(pkg) && !isOutdated(pkg))
   // {
   //    printf("\n%s is already installed and up-to-date.\nDoing nothing.\n\n", pkg);
   //    return 1;
   // }

   headList = createPackage(pkg, isExplicit(trimString(pkg)));
   if(headList->isExplicit){
      Explicit = Explicit + strlen(headList->pkg);
   }
   else{
      Depends = Depends + strlen(headList->pkg);
   }
   Package *whereAmI = headList;

   while(whereAmI != NULL){
      updateDependencyPackages(whereAmI->pkg);
      whereAmI = whereAmI->next;
   }

   // Package *tempPkgList = NULL;
   // int asDepsSize = 0;
   // int notAsDepsSize = 0;
   // while (whereAmI != NULL)
   // {
   //    if(strcmp(whereAmI->pkg, "tzdata") == 0){
   //       printf("Hi\n");
   //    }
   //    printf("%s\n", whereAmI->pkg);
   //    tempPkgList = getDependsOn(whereAmI->pkg);
   //    addUniquePackages(head, tempPkgList);
   //    tempPkgList = getOptionalDepsInstalled(whereAmI->pkg);
   //    addUniquePackages(head, tempPkgList);
   //    tempPkgList = getRequiredBy(whereAmI->pkg);
   //    addUniquePackages(head, tempPkgList);
   //    tempPkgList = getOptionalFor(whereAmI->pkg);
   //    addUniquePackages(head, tempPkgList);
   //    if(whereAmI->asdep){
   //       asDepsSize = asDepsSize + strlen(whereAmI->pkg);
   //    }
   //    else{
   //       notAsDepsSize = notAsDepsSize + strlen(whereAmI->pkg);
   //    }
   //    whereAmI = whereAmI->next;
   // }

   int buff = 1000;
   Depends = Depends + buff;
   Explicit = Explicit + buff;

   char *pDepends = (char *)malloc(Depends);
   char *pExplicit = (char *)malloc(Explicit);
   pDepends[0] = '\0';
   pExplicit[0] = '\0';

   whereAmI = headList;
   while (whereAmI != NULL)
   {
      if(!whereAmI->isExplicit){
         strcat(pDepends, whereAmI->pkg);
      }
      else{
         strcat(pExplicit, whereAmI->pkg);
      }
      whereAmI = whereAmI->next;
   }
   

   return 0;
}