#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define TEMP_FILE_NAME "test.txt"
#define BUFF_SIZE 1024
char readdata[BUFF_SIZE];
char writedata[BUFF_SIZE];

int main(void)
{
    char input[8];
    int breakloop = 0;


    while(!breakloop)
    {
        
        int fd = open(TEMP_FILE_NAME, O_RDWR);
        if(fd ==-1)
        {
            perror("Open to write\n");
            exit(1);
        }

        printf("please enter the operation(w/r/q)");
        scanf("%s", input);

        switch(input[0])
        {
            case 'q':
                breakloop =1;
                break;

            case 'w':
                printf("write : ");
                scanf("%s", writedata);
                int wd = write(fd, writedata, BUFF_SIZE);
                if(wd ==-1)
                {
                    perror("Fail to write\n");
                    exit(1);
                }
                break;

            case 'r':
                read(fd, &readdata, BUFF_SIZE);
                printf("read : %s\n", readdata);
                break;
        }
        close(fd);
    }  
    return 0;
}