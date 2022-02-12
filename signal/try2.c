/* OPERATING SYSTEM PROJECT NO.1
 * SOFTWARE LEE JINHEE 201723270
 * DUE DATA OCTOBER 27TH
 * --------------------------------
 * - Attemp : try2.c              -
 * --------------------------------
 */
#include <stdio.h>
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
#define FIFO_PATH "/tmp/fifo"
#define BUFF_SIZE 1024
#define MKFIFO_FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROT)

/* -------------------------------------------------------------------------------
 * DATA STORAGE
 * FILENAME, FILE ACCESS TYPE(r/w), NUMBER OF BYTES, DATA STRING
 * FILE_PATH, FILE_MODE??????????????
 * -------------------------------------------------------------------------------
 */
typedef struct DataBox{
    char* file_name;
    char* file_access_type;
    int number_of_bytes;
    char* data_string;
}DataBox;

/* act of function!!!!
 * server - receiver, client - sender
 */
void client(int writefd, DataBox data);
void server(int readfd);

/* -------------------------------------------------------------------------------
 * return of MKFIFO / success : 0 , failed : -1
 * -------------------------------------------------------------------------------
 * ERRNO IS ERRNO NUMBER / EEXIST IS FILE EXISTS http://blueheartscabin.blogspot.com/2013/09/unix-c-errno.html
 * -------------------------------------------------------------------------------
 * RETURN OF UNLINKED / success : 0 , failed : -1 (STORE AT ERRNO) https://downman.tistory.com/149
 * -------------------------------------------------------------------------------
 * THE INPUT : /tmp/fifo w 10 helloworld
 */
int main(int argc, char*argv[])
{
    DataBox data = {argv[1], argv[2], argv[3], argv[4]};
    int writefd;    int readfd;

    printf("%s\n", data.file_access_type);
    if((mkfifo(data.file_name, MKFIFO_FILE_MODE)<0))
    {
        fprintf(stderr, "FAIL MKFIFO()\n");
        return -1;
    }

/* -------------------------------------------------------------------------------
 * return of read(), size_t read(int fd, void * buff, size_t nbytes);
 * success : send word byte, failed : -1
 * -------------------------------------------------------------------------------
 * return of write(), size_t write(int fd, const void*buff, size_t nbytes);
 * success : read word byte, failed : -1
 * -------------------------------------------------------------------------------
 * return of open(), open(data.filename, access);
 * success : file discriptor , failed : -1 /https://downman.tistory.com/67
 */
    if((readfd = open(data.file_name, O_RDONLY))==-1)
    {
        fprintf(stderr, "FAIL TO OPEN()\n");
        return -1;
    }
    printf("read open\n");
    
    if((writefd = open(data.file_name, O_WRONLY|O_CREAT|O_TRUNC, 0777)==-1))
    {
        fprintf(stderr, "FAIL TO OPEN()\n");
        return -1;
    }
    printf("write open\n");
 
    client(writefd, data);
    server(readfd);

    return 0;
}

void client(int writefd, DataBox data)
{

    if(write(writefd, &data, sizeof(data))!=-1)
    {
        fprintf(stderr, "WRITE FILE FAILED\n");
        return -1;
    }
    printf("WRITE OPERATION SUCCESS\n");
    close(writefd);
}

void server(int readfd)
{
    DataBox server_data;

    if(read(readfd, &server_data, sizeof(server_data))==-1)
    {
        fprintf(stderr, "READ FILE FAILED\n");
        return -1;
    }
    printf("File_name : %s\n", server_data.file_name);
    printf("File_accesss_type : %s\n", server_data.file_access_type);
    printf("Number_of_bytes : %d\n", server_data.number_of_bytes);
    printf("Data_string : %s\n", server_data.data_string);
    printf("READ OPERATION SUCCESS\n");
    close(readfd);
}
