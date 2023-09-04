#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {

    /* filename */

    char filename[100];

    /* file descriptor */

    int fd;

    /* read buffer */

    char buffer[100];

    /* retval */

    int status;


    /* read the input filename */

    status = scanf("%s", filename);


    /* open file using open, in read-only mode*/


    /* read file using read*/


    /* write first byte from buffer to stdout using write*/


    /* close file*/

    close(fd);    
    
    return 0;

}