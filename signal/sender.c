#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define  FIFO_FILE   "/tmp/fifo1"

int main( int argc, char*argv[]) 
{
   int fd;
   char *str = argv[1];
   if ( -1 == ( fd = open( FIFO_FILE, O_WRONLY)))
   {
      perror( "open() failed\n");
      return -1;
   }
   printf("FD=%d\n", fd);
   write(fd, str, strlen(str));
   close(fd);
   return 0;
}

