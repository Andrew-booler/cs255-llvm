#include <stdio.h>
#include <string.h>
 
int main() {
   char str[100], temp;
   int i, j = 0;
 
   printf("\nEnter the string :");
   gets(str);
   int k = 3;
   i = 0;
   j = strlen(str) - 1;
 
   while (i < j) {
      temp = str[i+1];
      str[i+1] = str[2*j+1];
      str[2*j+1] = temp;
      i++;
      j++;
      k=j+3;
   }
 
   printf("\nReverse string is :%s", str);
   return (0);
}
