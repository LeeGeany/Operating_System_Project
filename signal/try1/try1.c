/* OPERATING SYSTEM PROJECT NO.1
 * SOFTWARE LEE JINHEE 201723270
 * DUE DATA OCTOBER 27TH
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
#define FIFO_PATH "/tmp/fifo.1"
#define BUFF_SIZE 1024
#define MODE 0777
/* DATA STORAGE
 * FILENAME, FILE ACCESS TYPE, NUMBER OF BYTES, DATA STRING
 * FILE_PATH, FILE_MODE??????????????
 */
typedef struct _mailbox{
    char*filename;
    char*file_access_type;
    int number_of_bytes;
    char*data_string;
}MailBox;

/* return of MKFIFO / success : 0 , failed : -1
* ----------------------------------------
* ERRNO IS ERRNO NUMBER / EEXIST IS FILE EXISTS http://blueheartscabin.blogspot.com/2013/09/unix-c-errno.html
*----------------------------------------
* RETURN OF UNLINKED / success : 0 , failed : -1 (STORE AT ERRNO) https://downman.tistory.com/149
*/
int main()
{
    MailBox *mb;

    /*if(argc != 4)
    {
        fprintf(stderr, "usage : %s <filename> <access mode> <number of btyes> <data string(if read)>\n", argv[0]);
        fprintf(stderr, "acess mode is octect number\n");
        return -1;
    }*/


    while(1)
    {
        printf("Running...");
        client();
        server();

    }

    return 0;
}

void client()
{
    int counter  =0;
    int writefd;
    char* string = "TEST FIFO IPC" ;
    
    if((writefd = open(FIFO_PATH, O_WRONLY, MODE)==-1))
    {
        fprintf(stderr, "OPEN FILE FAILED\n");
        exit(1);
    }
    
    if(write(writefd, string, strlen(string))<0)
    {
        fprintf(stderr, "WRITE FILE FAILED\n");
        exit(1);
    }
    else
    {
        fprintf(stderr, "WRITE SUCCESS\n");
    }
}

void server()
{
    int counter =0;
    int readfd;
    char buff[BUFF_SIZE];

    if((mkfifo(FIFO_PATH, FILE_MODE)<0) &&(errno != EEXIST))
    {
        printf("can't create %s", FIFO_PATH);
        exit(1); 
    }

    if((readfd = open(FIFO_PATH, O_RDWR, MODE)==-1))
    {
        fprintf(stderr, "OPEN FILE FAILED\n");
        exit(1);
    }

    while(1)
    {
        memset(buff, 0x00, BUFF_SIZE);
        read(readfd, buff, BUFF_SIZE);
        printf("%d : %s\n", counter++, buff);
    }

}
