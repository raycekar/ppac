#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      printf("Error: ppac only accepts one agument.\n");
      return 1;
   }
   Package *head = NULL;
   head = (Package *)malloc(sizeof(Package));
   head->pkg = (char*)malloc(sizeof(argv[1]));
   strcpy(head->pkg, argv[1]);
   head->asdep = FALSE;
   head->next = NULL;
   getDependsOn(argv[1]);
   // if (isPackageInstalled(argv[1]) == 1)
   // {
   //    getPackageDependencies(argv[1], head);
   // }

   printf("%d", isPackageInstalled(argv[1]));

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