#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Package {
   char *pkg;
   struct Package *next;
} Package;


const char* combineStrings(const char *first, const char *second){
   char *temp = (char *) malloc(strlen(first) + strlen(second) + 1);
   memcpy(temp, first, strlen(first));
   memcpy(temp + strlen(first), second, strlen(second) + 1);
   return temp;
}


int main(int argc, char *argv[]) {
   if (argc != 2){
      printf("Error: ppac only accepts one agument.\n");
      return 1;
   }
   const char *pacman = "pacman -Si ";
   const char *command = combineStrings(pacman, argv[1]);

   FILE *p;
   p = popen(command, "r");

   int ch;
   while( (ch=fgetc(p)) != EOF)
        putchar(ch);
    pclose(p);
   
   Package head;
   head->pkg = "hello";
   return 0;
}