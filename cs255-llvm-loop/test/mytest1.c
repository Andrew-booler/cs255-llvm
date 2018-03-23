#include <stdio.h>
#include <string.h>
#include <time.h>

int main(){

    time_t t;
    t=time(NULL);
    if(t%7==0){
        for (int i =0;i<=50;i++){
            printf("7");
        }
        printf("\n");
    }else{
    for (int i =0;i<=50;i++){
            printf("0");
        }
    printf("\n");
    }
    return 0;
}
