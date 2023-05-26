/* i n e t _ s t r _ s e r v e r . c : Internet stream sockets server */
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

/* Server */
void child_server(int newsock);
void sigchld_handler(int sig);

/* Client */

/* Both */
void perror_exit(char *message);