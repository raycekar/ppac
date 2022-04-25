#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <signal.h>

#include "Utils.h"

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      printf("Error: ppac only accepts one agument.\n");
      return 1;
   }
   // signal(SIGPIPE, SIG_IGN);
   printf("Initilizing...\n");
   syncDatabase();
   initilizeAll();
   char *pkg = (char *)malloc(strlen(argv[1]));
   strcpy(pkg, argv[1]);
   printf("Checking package...\n");
   if(!isInDatabase(pkg)){
      printf("%s was not found to be a valid package.\nDoing Nothing.\n", pkg);
      return 1;
   }
   if (isPackageInstalled(pkg) && !isOutdated(pkg))
   {
      printf("\n%s is already installed and up-to-date.\nDoing nothing.\n\n", pkg);
      return 1;
   }

   bool explicitPkg = TRUE;
   if(isPackageInstalled(pkg)){
      explicitPkg = isExplicit(trimString(pkg));
   }

   headList = createPackage(pkg, explicitPkg);
   if(headList->isExplicit){
      explicitWordLen = explicitWordLen + strlen(headList->pkg);
   }
   else{
      dependsWordLen = dependsWordLen + strlen(headList->pkg);
   }
   Package *whereAmI = headList;

   while(whereAmI != NULL){
      updateDependencyPackages(whereAmI->pkg);
      whereAmI = whereAmI->next;
   }

   char *pDepends = (char *)malloc(dependsWordLen + dependsWordCt);
   char *pExplicit = (char *)malloc(explicitWordLen + explicitWordCt);
   pDepends[0] = '\0';
   pExplicit[0] = '\0';

   whereAmI = headList;
   while (whereAmI != NULL)
   {
      if(!whereAmI->isExplicit){
         strcat(pDepends, whereAmI->pkg);
         strcat(pDepends, " ");
      }
      else{
         strcat(pExplicit, whereAmI->pkg);
         strcat(pExplicit, " ");
      }
      whereAmI = whereAmI->next;
   }
   
   printf("\nThe following dependencies will be updated:\n%s\n", pDepends);
   printf("\nThe following packages will be updated/installed:\n%s\n\n", pExplicit);

   char response;
   bool goAhead = FALSE;
   while(TRUE){
      printf("Proceed with installation? [Y/n]\n");
      response = getchar();
      if(response == 110 || response == 78){
         break;
      }
      if(response == 121 || response == 89 || response == 10){
         goAhead = TRUE;
         break;
      }
      printf("\nInvalid response.\n");
   }

   if(goAhead){
      unsigned int size;
      if(strlen(pDepends) > strlen(pExplicit)){
         size = strlen(pDepends);
      }
      else{
         size = strlen(pExplicit);
      }

      char *cmd = (char *)malloc(size + 100);
      if(strlen(pDepends) != 0){
         cmd[0] = '\0';
         strcat(cmd, PHS);
         strcat(cmd, "--asdeps --needed --noconfirm ");
         strcat(cmd, pDepends);
         system(cmd);
      }
      
      if(strlen(pExplicit) != 0){
         cmd[0] = '\0';
         strcat(cmd, PHS);
         strcat(cmd, "--needed --noconfirm ");
         strcat(cmd, pExplicit);
         system(cmd);
      }

      free(cmd);
   }

   free(pDepends);
   free(pExplicit);
   return 0;
}