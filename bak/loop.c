#include <stdio.h>  
#include <unistd.h>
  
int main(int argc, char *argv[])  
{  
    int result = 0;  
    int i;  
  
    printf("crtest starting with\n");  
  
    for (i = 0; i < 100; i++) {  
        result += 1;  
        printf("%d\n", result);  
        sleep(1);  
    }  
  
    return 0;  
}  
