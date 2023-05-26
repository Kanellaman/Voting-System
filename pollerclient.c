#include "Interface.h"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Not enough arguments, try  pollSwayer [serverName] [portNum] [inputFile.txt]\n");
        exit(-1);
    }
    int port = atoi(argv[2]), sock, i;
    char *name = argv[1], *stats = argv[3], buf[256];
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct hostent *rem;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(-1);
    }
    if ((rem = gethostbyname(name)) == NULL)
    {
        herror(" gethostbyname ");
        exit(1);
    }
    server.sin_family = AF_INET;   /* Internet domain */
    server.sin_port = htons(port); /* Server port */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
        perror_exit(" connect ");

    close(sock);
    return 0;
}