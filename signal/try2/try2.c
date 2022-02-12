/* OPERATING SYSTEM PROJECT NO.1
 * SOFTWARE LEE JINHEE 201723270
 * DUE DATA OCTOBER 27TH
 * --------------------------------
 * - Attemp : try2.c              -
 * --------------------------------
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
#define MKFIFO_FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define FILE_PATH "/tmp/fifo1"

/* -------------------------------------------------------------------------------
 * DATA STORAGE
 * FILENAME, FILE ACCESS TYPE(r/w), NUMBER OF BYTES, DATA STRING
 * FILE_PATH, FILE_MODE??????????????
 * -------------------------------------------------------------------------------
 */
typedef struct DataBox{
    char* file_name;
    char* file_access_type;
    char* number_of_bytes;
    char* data_string;
}DataBox;

/* act of function!!!!
 * server - receiver, client - sender
 */
void client(DataBox data);
void server(DataBox data);

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
    
    system("rm /tmp/fifo1 -f");



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
    while(1)
    {
        server(data);
        client(data);
        
    }
    return 0;
}

client(DataBox data)
{
    DataBox client_data;

    client_data.file_name= data.file_name;
    client_data.file_access_type = data.file_access_type;
    client_data.number_of_bytes = data.number_of_bytes;
    client_data. data_string = data.data_string;
    printf("store success\n");

    int writefd = open(FILE_PATH, O_WRONLY, 0777);
    if(writefd == -1)
    {
        perror("FAIL TO OPEN()\n");
        exit(1);
    }
    printf("open success\n");

    int wd = write(writefd, &client_data, sizeof(client_data));
    if(wd==-1)
    {
        fprintf(stderr, "WRITE FILE FAILED\n");
        exit(1);
    }
    printf("SUCCESS TO WRITE THE %d BYTES\n", wd);
    close(writefd);
}

server(DataBox data)
{
    DataBox server_data;

    int fifo = mkfifo(FILE_PATH, 0666);
    if(fifo==-1)
    {
        fprintf(stderr, "FAIL MKFIFO()\n");
        return -1;
    }

    int readfd = open(FILE_PATH, O_RDONLY);
    if(readfd ==-1)
    {
        fprintf(stderr, "FAIL TO OPEN()\n");
        exit(1);
    }

    int rd =read(readfd, &server_data, sizeof(server_data));
    if(rd==-1)
    {
        fprintf(stderr, "READ FILE FAILED\n");
        exit(1);
    }

    while(1)
    {
        printf("File_name : %s\n", server_data.file_name);
        printf("File_accesss_type : %s\n", server_data.file_access_type);
        printf("Number_of_bytes : %s\n", server_data.number_of_bytes);
        printf("Data_string : %s\n", server_data.data_string);
        printf("READ OPERATION SUCCESS\n");
        close(readfd);
    }
}
