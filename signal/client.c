/* OPERATING SYSTEM PROJECT NO.1
 * SOFTWARE LEE JINHEE 201723270
 * DUE DATA OCTOBER 27TH
 * --------------------------------
 * -    CLIENT                    -
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

int main(int argc, char * argv[])
{
    int writefd;
    char * string = argv[1];

    /* return of MKFIFO / success : 0 , failed : -1
     * ----------------------------------------
     * ERRNO IS ERRNO NUMBER / EEXIST IS FILE EXISTS http://blueheartscabin.blogspot.com/2013/09/unix-c-errno.html
     *----------------------------------------
     * RETURN OF UNLINKED / success : 0 , failed : -1 (STORE AT ERRNO) https://downman.tistory.com/149
     *----------------------------------------
     * DATA STORAGE
     * FILENAME, FILE ACCESS TYPE(r/w), NUMBER OF BYTES, DATA STRING
     * FILE_PATH, FILE_MODE??????????????
     */

    /*if(!(argc == 4 || argc ==5))
    {
        fprintf(stderr, "usage : %s <filename> <access mode> <number of btyes> <data string(if write)>\n", argv[0]);
        fprintf(stderr, "acess mode is octect number\n");
        return -1;
    }*/

    if((writefd = open(FIFO_PATH, O_WRONLY)==-1))
    {
        fprintf(stderr, "OPEN FILE FAILED\n");
        return -1;
    }
        
    if(write(writefd, string ,strlen(string))<0)
    {
        fprintf(stderr, "WRITE FILE FAILED1\n");
        return -1;
    }
    printf("succuss");
    close(writefd);
    
    return 0;
}