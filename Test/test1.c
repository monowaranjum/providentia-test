#include <fcntl.h> 
#include <stdio.h> 
#include <string.h>
#include <unistd.h> 

// int main(int argc, char const *argv[])
// {
//     char name[20] = "input.txt";
//     FILE *fp;
//     fp = fopen(name, "r");
//     char buffer[1024];
//     for(int i=0;i<3;i++){
//         int count = fread(buffer, 1024, 1,fp);
//         printf("%s\n", buffer);
//     }
//     fclose(fp);
//     return 0;
// }

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
    
    bytes_read = read(fd, buffer, 9);

    bytes_read = read(fd2, buffer, 9); 

    bytes_read = read(fd2, buffer, 9); 

    bytes_read = read(fd, buffer, 9);


    
    close(fd);

    close(fd2);

    return 0;
}

