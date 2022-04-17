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
   //signal(SIGPIPE, SIG_IGN);
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

   head->next = NULL;
   getDependsOn(pkg);

   char x[] = "i3-gaps";
   getOptionalDepsInstalled(x);
   // // if (isPackageInstalled(argv[1]) == 1)
   // // {
   // //    getPackageDependencies(argv[1], head);
   // // }

   // printf("%d", isPackageInstalled(argv[1]));

   // const char *pacman = "pacman -Qi ";
   // const char *command = combineStrings(pacman, argv[1]);

   // FILE *p;
   // p = popen(command, "r");

   // int ch;
   // while( (ch=fgetc(p)) != EOF)
   //      putchar(ch);
   //  pclose(p);

   // Package head;
   // strcpy(head.pkg, "hello");
   return 0;
}