#include "Interface.h"

void child_server(int newsock)
{
    char buf[1];
    while (read(newsock, buf, 1) > 0)
    {                    /* Receive 1 char */
        putchar(buf[0]); /* Print received char */
        /* Capitalize character */
        buf[0] = toupper(buf[0]);
        /* Reply */
        if (write(newsock, buf, 1) < 0)
            perror_exit(" write ");
    }
    printf(" Closing connection .\n ");
    close(newsock); /* Close socket */
}
/* Wait for all dead child processes */
void sigchld_handler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}
void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}