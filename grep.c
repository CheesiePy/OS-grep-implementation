#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Usage: %s <pattern> <file> [if file path not provided grep will search in stdin]\n", argv[0]);
    }
    printf("Pattern: %s\n", argv[1]);

    if (argc == 3){ // we got a file name from user
        // using only system calls to read the file
        int fd = open(argv[2], O_RDONLY);
        if (fd == -1){
            perror("Error opening file");
            return 1;
        }
        
    }
}