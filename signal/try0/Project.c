#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXLINE 4096
#define STDOUT_FILENO 1
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define MODE 0777

/* FILE OF PERMISSION BITS
 * S_IRUSR : USER CAN READ FILE     S_IWUSR : USER CAN WRTIE FILE
 * S_IRGRP : GROUP CAN READ FIEL    S_IROTH : ETC READ
 * MORE ABOUT : https://devanix.tistory.com/289
 */
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

typedef struct _mailbox{
    char*filename;
    char*file_access==type;
    int number_of_bytes;
    char*data_string;
}MailBox;

void server(int readfd, int writefd);
void client(int readfd, int writefd, MailBox*mb);

int main(int argc, char * argv[])
{
    int readfd;     int writefd;
    pid_t childpid;
    mode_t mode;

    /* return of MKFIFO / success : 0 , failed : -1
     * ----------------------------------------
     * ERRNO IS ERRNO NUMBER / EEXIST IS FILE EXISTS http://blueheartscabin.blogspot.com/2013/09/unix-c-errno.html
     *----------------------------------------
     * RETURN OF UNLINKED / success : 0 , failed : -1 (STORE AT ERRNO) https://downman.tistory.com/149
     */

    if(argc != 4)
    {
        fprintf(stderr, "usage : %s <filename> <access mode> <number of btyes> <data string(if read)>\n", argv[0]);
        fprintf(stderr, "acess mode is octect number\n");
        return -1;
    }
    

    if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST))  
    {
       printf("can't create %s", FIFO1);
       exit(1); 
    }

    if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST))     
    {      
        unlink(FIFO1);     
        printf("can't create %s", FIFO2);   
        exit(1);
    }   

    if((childpid = fork())== 0)
        readfd = open(FIFO1, O_RDONLY, MODE );
        writefd = open(FIFO2, O_WRONLY|O_CREAT|O_TRUNC, MODE);

        server(readfd, writefd);
        exit(0);
    
    readfd = open(FIFO1, O_RDONLY, MODE );
    writefd = open(FIFO2, O_WRONLY|O_CREAT|O_TRUNC, MODE);

    client(readfd, writefd, &mb);

    /* return of waitpid()
     * success : Process ID , failed : -1 /https://codetravel.tistory.com/42
     */
    waitpid(childpid, NULL, 0);

}

void client(int readfd, int writefd, MailBox* mb)
{
    size_t len;
    size_t n;

    char buff[MAXLINE] = mb->data_string;
    len = strlen(buff);

    if(buff[len-1]== '\n') 
        len--;

    write(writefd, buff, len);
    /* return of read()
     * success : send word byte, failed : -1
     */
    while((n=read(readfd, buff, MAXLINE))>0)
        write(STDOUT_FILENO, buff, n);
        pritnf("%s", buff);
}

void server(int readfd, int writefd)
{
    int fd;
    size_t n;
    char buff[MAXLINE+1];

    if((n=read(readfd, buff, MAXLINE))==0)
        printf("end of file");
        exit(0);
    buff[n] ='\0';

    /* return of open()
     * success : file discriptor , failed : -1 /https://downman.tistory.com/67
     */
    if((fd = open(buff, O_RDONLY))<0)
    {
        snprintf(buff+n, sizeof(buff)-n, "can't open %s\n", strerror(errno));
        n= strlen(buff);
        write(writefd, buff, n);
    }
    else
    {
        while((n=read(fd,buff,MAXLINE))>0)
            write(writefd,buff, n);
        close(fd);
    }
}