#include <fcntl.h> 
#include <stdio.h> 
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    int fd;
    char fileName[20] = "input.txt";
    char fileName2[20] = "input2.txt";
    if(access(fileName, F_OK)==0){
        fd = open(fileName, O_RDONLY);
    }
    else{
        return 0;
    }
    int fd2 = open(fileName2, O_RDONLY);
    unsigned char buffer[10];
    int bytes_read = 1;
    int k=0;
    
    int count = 3;
    while(count--){
        bytes_read = read(fd, buffer, 9);

        
        bytes_read = read(fd, buffer, 9);

        int idx = 4;
        while(--idx){
            bytes_read = read(fd2, buffer, 9); 

            bytes_read = read(fd2, buffer, 9); 
        }
    }


    
    close(fd);

    close(fd2);

    return 0;
}

