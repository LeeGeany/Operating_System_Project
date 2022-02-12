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
#include <wait.h>
#include <string.h>

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
#define MODE 0777
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
 * return of read(), size_t read(int fd, void * buff, size_t nbytes);
 * success : send word byte, failed : -1
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
 * THE INPUT : /tmp/fifo w 10 helloworld
 */
int main()
{   
    while(1)
    {
        DataBox server_data;
        pid_t pid;
        char bytes[10];

        system("rm /tmp/fifo -f");

        int fifo = mkfifo(FILE_PATH, 0666);
        if(fifo==-1)
        {
            fprintf(stderr, "FAIL MKFIFO()\n");
            return -1;
        }

        int readfd = open(FILE_PATH, O_RDONLY, 0777);
        if(readfd ==-1)
        {
            fprintf(stderr, "FAIL TO OPEN()\n");
            exit(1);
        }

        int rd = read(readfd, &server_data, sizeof(server_data));
        if(rd == -1)
        {
            fprintf(stderr, "READ FILE FAILED\n");
            exit(1);
        }
        /* -------------------------------------------------------------------------------
        * PROCESS CONTROL ::: MAKE CHILD PROCESS
        * FORK() - MAKE CHILD PROCESS
        * PID == -1 : ERROR, PID == 0 : CHILD PROCESS, PID ELSE : PARENTS PROCESS
        * https://lagifun-inforecord.tistory.com/28
        * -------------------------------------------------------------------------------
        * TYPE OF EXEC FUNCTION 
        * EXECV : execv("path", parmeter) parmeter (ex) ls -a 0 // 0 means end 
        *
        */
        
        sprintf(bytes, "%ld", strlen(server_data.string_or_bytes));

        pid = fork();
        if(pid == -1)
        {
            fprintf(stderr, "FAIL TO FORK()\n");
            exit(1);
        }
        else if(pid == 0)
        {   
            if( strcmp(server_data.file_access_type, "r")==0)
            {
            // CHILD PROCESS RUNNING...
                printf("Chlid Read Running...\n");
                execl("/usr/bin/head", "head", "-c", server_data.string_or_bytes, server_data.file_name, NULL);    
            }
            else if( strcmp(server_data.file_access_type,"w")==0)
            {
               
                printf("Child Write Running...\n");
                execl("/bin/echo","echo ", bytes, NULL);
            }
            else
            {
                printf("NOT AVAILABLE COMMAND\n");
                exit(1);
            }
        }
        else
        {
            // PARENTS PROCESS RUNNING...
            printf("\nparents\n");
            printf("File_name : %s\n", server_data.file_name);
            printf("File_acess_type : %s\n", server_data.file_access_type);
            printf("string_or_bytes : %s\n", server_data.string_or_bytes);
            sleep(1);
            wait(NULL);
        }
        close(readfd);
    }    
}