#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Package {
   char pkg[50];
   struct Package *next;
} Package;


const char* combineStrings(const char *first, const char *second){
   char *temp = (char *) malloc(strlen(first) + strlen(second) + 1);
   memcpy(temp, first, strlen(first));
   memcpy(temp + strlen(first), second, strlen(second) + 1);
   return temp;
}

int isPackageInstalled(char *pkg){
   char cmd[100] = "pacman -Qi ";
   strcat(cmd, pkg);
   char output[100];
   output[0] = '\0';
   FILE *p;
   p = popen(cmd, "r");
   fgets(output, 100, p);
   if(
      output[0] == 'N' &&
      output[1] == 'a' &&
      output[2] == 'm' &&
      output[3] == 'e'
      ){
      return 1;
   }
   return 0;

}

int main(int argc, char *argv[]) {
   if (argc != 2){
      printf("Error: ppac only accepts one agument.\n");
      return 1;
   }

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