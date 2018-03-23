#include <stdio.h>
#include <string.h>
 
#define MAX_CHAR 26
#define MAX_LENGTH 1024

// Driver code
int main()
{

   int k = 2;
   int l =k;
   // Taking an extra array to keep
   // record for character count in s
   int a[MAX_CHAR] = { 0 };

   char *str;

   // Counting occurrences of all 
   // characters in str[]
   for (int tesn = 1; tesn< MAX_LENGTH; tesn++)
        l-=2;
   if(l>=-2048)
        printf("l is lower than -2048\n");
   for (int i = 0; i < MAX_LENGTH; i++) 
     a[str[i] - 'a']++;    

   // Printing characters with count
   // >= k in same order as they appear
   // in str.
   for (int i = 0; i < MAX_LENGTH; i++) 
      if (a[str[i] - 'a'] >= k)
         printf("%c\n", str[i]); 

   return 0;
}
