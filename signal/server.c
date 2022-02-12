/* OPERATING SYSTEM PROJECT NO.1
 * SOFTWARE LEE JINHEE 201723270
 * DUE DATA OCTOBER 27TH
 * --------------------------------
 * -    SERVER                    -
 * --------------------------------
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

/* FILE OF PERMISSION BITS
 * S_IRUSR : USER CAN READ FILE     S_IWUSR : USER CAN WRTIE FILE
 * S_IRGRP : GROUP CAN READ FIEL    S_IROTH : ETC READ
 * MORE ABOUT : https://devanix.tistory.com/289
 * ----------------------------------------
 * FILE OF PERMISSION BITS(NUM.VER)
 * owner, group, others
 *        -rwx -rwx -rwx
 * 0777 -> 0111 0111 0111
 * 0666 -> 0110 0110 0110
 */

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define FIFO_PATH "/tmp/fifo"
#define BUFF_SIZE 1024

int main()
{
    int readfd;
    char buff[BUFF_SIZE];

    if((mkfifo(FIFO_PATH, 0666)==-1))
    {
        printf("can't create %s", FIFO_PATH);
        return -1; 
    }

    if((readfd = open(FIFO_PATH, O_RDWR)==-1))
    {
        fprintf(stderr, "OPEN FILE FAILED\n");
        return -2;
    }
    
        
    while(1)
    {
        memset(buff, 0x00, BUFF_SIZE);
        read(readfd, buff, BUFF_SIZE);
        printf("read : %s\n", buff);
        //fork
    }
    close(readfd);
    return 0;
}