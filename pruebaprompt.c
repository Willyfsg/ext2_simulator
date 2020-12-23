#include <stdio.h>

int main()
{
  char mychar[30];
  char arg[30];
    while(1){
        printf("Grock- ");
        fscanf (stdin, "%s", mychar);
        fgets (arg, 100, stdin);
        printf("Command entered: %s\n", mychar); 
        printf("Arguments entered: %s\n", arg);

    }
}