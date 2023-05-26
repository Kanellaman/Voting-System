#include "Interface.h"

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        printf("Not enough arguments, try poller [portnum] [numWorkerthreads] [bufferSize] [poll-log][poll-stats]\n");
        exit(-1);
    }
    int port = atoi(argv[1]), threads = atoi(argv[2]), buffersize = atoi(argv[3]);
    char *log = malloc(strlen(argv[4]) * sizeof(char) + 1), *stats = malloc(strlen(argv[5]) * sizeof(char) + 1);
    strcpy(log, argv[4]);
    strcpy(stats, argv[5]);
    return 0;
}