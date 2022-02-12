/* OPERATING SYSTEM PROJECT NO.1
 * SOFTWARE LEE JINHEE 201723270
 * DUE DATA OCTOBER 27TH
 *
 * ------------------------------------------------------------------
 * - Attempt3 : server.c                                            -
 * ------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

/* -------------------------------------------------------------------------------
 * FILE OF PERMISSION BITS
 * S_IRUSR : USER CAN READ FILE     S_IWUSR : USER CAN WRTIE FILE
 * S_IRGRP : GROUP CAN READ FIEL    S_IROTH : ETC READ
 * MORE ABOUT : https://devanix.tistory.com/289
 * -------------------------------------------------------------------------------
 * FILE OF PERMISSION BITS(NUM.VER)
 * owner, group, others
 *         -rwx -rwx -rwx
 * 0777 -> 0111 0111 0111
 * 0666 -> 0110 0110 0110
 * -------------------------------------------------------------------------------
 */
#define FILE_PATH "/tmp/fifo"
#define MODE 0666
#define MAX_LEN 1024
/* -------------------------------------------------------------------------------
 * DATA STORAGE
 * FILENAME, FILE ACCESS TYPE(r/w), NUMBER OF BYTES, DATA STRING
 * FILE_PATH, FILE_MODE??????????????
 * -------------------------------------------------------------------------------
 */
typedef struct DataBox{
    char file_name[MAX_LEN];
    char file_access_type[2];
    char string_or_bytes[MAX_LEN];
}DataBox;

/* -------------------------------------------------------------------------------
 * return of write(), size_t write(int fd, const void*buff, size_t nbytes);
 * success : read word byte, failed : -1
 * -------------------------------------------------------------------------------
 * return of open(), open(const char* pathname, int flags);
 * success : file discriptor , failed : -1 /https://downman.tistory.com/67
 * -------------------------------------------------------------------------------
 * return of MKFIFO / success : 0 , failed : -1
 * -------------------------------------------------------------------------------
 * ERRNO IS ERRNO NUMBER / EEXIST IS FILE EXISTS http://blueheartscabin.blogspot.com/2013/09/unix-c-errno.html
 * -------------------------------------------------------------------------------
 * RETURN OF UNLINKED / success : 0 , failed : -1 (STORE AT ERRNO) https://downman.tistory.com/149
 * -------------------------------------------------------------------------------
 * THE INPUT : test w 8 helloworld
 */

int main(int argc, char* argv[])
{   
    if(!(argc ==4))
    {   
        fprintf(stderr, "If you want write\n");
        fprintf(stderr, "Usage : %s <filename> w <number of btyes>\n", argv[0]);
        fprintf(stderr, "If you want read\n");
        fprintf(stderr, "Usage : %s <filename> r <data string>\n", argv[0]);
        return -1;
    }

    DataBox client_data[1];
    sscanf( argv[1], "%s" ,client_data->file_name);
    sscanf( argv[2], "%s", client_data->file_access_type);
    sscanf( argv[3], "%s", client_data->string_or_bytes);

    int writefd = open(FILE_PATH, O_WRONLY, 0777);
    if(writefd == -1)
    {
        perror("FAIL TO OPEN()\n");
        exit(1);
    }

    int wd = write(writefd, &client_data, sizeof(client_data));
    if(wd==-1)
    {
        fprintf(stderr, "WRITE FILE FAILED\n");
        exit(1);
    }
    printf("SUCCESS\n");
    close(writefd);
}
