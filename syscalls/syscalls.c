#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

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

    /* open file using open, in read-only mode */
    fd = open(filename, O_RDONLY);

    /* read file using read */
    read(fd, buffer, sizeof(buffer));

    /* write the first byte from buffer to stdout using write */
    write(STDOUT_FILENO, buffer, 1);
    //printf("\n");

    /* close file */
    close(fd);

    return 0;
}
