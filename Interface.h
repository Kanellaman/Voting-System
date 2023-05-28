#include <stdio.h>
#include <sys/wait.h>  /*sockets*/
#include <sys/types.h> /*sockets*/
#include <sys/socket.h>/*sockets*/
#include <netinet/in.h>/*internetsockets*/
#include <netdb.h>     /*gethostbyaddr*/
#include <unistd.h>    /*fork*/
#include <stdlib.h>    /*exit*/
#include <ctype.h>     /*toupper*/
#include <signal.h>    /*signal */
#include <string.h>
#include <pthread.h>
#include <fcntl.h>


/* Both */
void perror_exit(char *message, int socket, void *dealloc);