#include "Interface.h"

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        printf("Not enough arguments, try poller [portnum] [numWorkerthreads] [bufferSize] [poll-log][poll-stats]\n");
        exit(-1);
    }
    int port = atoi(argv[1]), threads = atoi(argv[2]), buffersize = atoi(argv[3]), sock, newsock;
    char *log = malloc(strlen(argv[4]) * sizeof(char) + 1), *stats = malloc(strlen(argv[5]) * sizeof(char) + 1), buf[1024];
    strcpy(log, argv[4]);
    strcpy(stats, argv[5]);
    struct sockaddr_in server, client;
    socklen_t clientlen;
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct sockaddr *clientptr = (struct sockaddr *)&client;
    struct hostent *rem;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit(" socket ");
    server.sin_family = AF_INET; /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port); /* The given port */

    if (bind(sock, serverptr, sizeof(server)) < 0)
        perror_exit(" bind ");
    if (listen(sock, 5) < 0)
        perror_exit(" listen ");

    printf(" Listening for connections to port %d \n ", port);

    if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
        perror_exit("accept");
    printf(" Accepted connection\n ");
    close(newsock); /* parent closes socket to client */
    close(sock);
    return 0;
}