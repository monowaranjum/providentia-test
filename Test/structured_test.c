#include <fcntl.h> 
#include <stdio.h> 
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>


int main()
{
    FILE *fp0 = fopen("input.txt", "r");
    FILE *fp1 = fopen("input2.txt", "w");
    FILE *fp2 = fopen("input3.txt", "w");
    int count = 3;
    char *buffer = (char *)malloc(sizeof(char)*1024);
    while(--count){
        count = fread(buffer, 1024, 1,fp0);
        fseek(fp0,1, SEEK_SET);
    }

    while(count){
        if(count%2){
            int idx = 3;
            while(--idx){
                fwrite(buffer, 1024, 1, fp1);
            }
        }
        else{
            int idx = 3;
            while(--idx){
                fwrite(buffer, 1024, 1, fp2);
            }
        }
        count--;
    }

    fclose(fp0);
    fclose(fp1);
    fclose(fp2);
    
    return 0;
}
