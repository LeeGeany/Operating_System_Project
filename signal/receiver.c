#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define  FIFO_FILE   "/tmp/fifo1"
#define  BUFF_SIZE   1024

int main( void) 
{
   int counter = 0;
   int fd;
   char buff[BUFF_SIZE];
   if(mkfifo(FIFO_FILE, 0666) == -1)
   {
      perror( "mkfifo() failed\n");
      return -1;
   }
   if (( fd = open( FIFO_FILE, O_RDWR)) == -1)
   {
      perror( "open() failed\n");
      return -2;
   }
   
   printf("FD=%d\n", fd);
   while( 1 )
   {
      memset( buff, 0x00, BUFF_SIZE);
      read( fd, buff, BUFF_SIZE);
      printf( "%d: %s\n", counter++, buff);
   }
   close(fd);
   return 0; 
}
