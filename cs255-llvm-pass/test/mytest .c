#include <stdio.h>
#include <string.h>

void welcome(char* name){
   printf("\nwelcome to the program: %s", name);
}

int calculator(int a, int b, char cal){
   if(cal=='+'){
      return a+b;
   }else{
	if(cal=='-'){
	   return a-b;
	}else{
		if(cal=='*'){
			return a*b;
		}else{ 
			if(cal=='/'){
				return a/b;
			}else{
				printf("\noperater invalid");
				return 0;
			}
		}
	}
   }
}
 
int main() {
   welcome("cs255");

   char str[100], temp;
   int i, j = 0;
 
   printf("\nEnter the string :");
   gets(str);
 
   i = 0;
   j = strlen(str) - 1;
    
   while (i < j) {
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
      i++;
      j--;
   }
 
   printf("\nReverse string is :%s", str);
   printf("\n 255 - 128 = %d",calculator(255, 128,'-')); 
   return (0);
}
