#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER      'J'
#define IOCTL_CMD_READ _IOR     (IOCTL_MAGIC_NUMBER,1, int)
#define IOCTL_CMD_WRITE _IOW    (IOCTL_MAGIC_NUMBER,2 ,int)

int main(void)
{
    int fd;
    char number[1024];
    int breakloop =0;
    char input[8];

    fd = open("/dev/ioctl", O_RDWR);
    if(fd <0)
    {
        perror("Failed to open the device\n");
    }

    while(!breakloop)
    {
        printf("please enter the operation(w/r/q)");
        scanf("%s", input);

        switch(input[0])
        {
            case 'q':
                breakloop =1;
                break;
            case 'w':
                printf("Enter the data to write: ");
                scanf("%s", number);
                ioctl(fd, IOCTL_CMD_WRITE, &number);
                break;
            case 'r':
                ioctl(fd, IOCTL_CMD_READ, &number);
                printf("The data in device is %s\n", number);
                break;
        }
    }
    close(fd);
    return 0;
}
