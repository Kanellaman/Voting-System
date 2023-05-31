#include "../include/Interface.h"
#include "../include/InterfaceServer.h"

void sig_handler(int sig)
{                                  /* Handler of the signal SIGINT- Freeing memory/Printing results/Waiting for gracefully closing of all the threads */
    flag = 1;                      // Message for threads to terminate
    pthread_cond_broadcast(&cond); // Signal everyone waiting at the condition variable cond
    for (int i = 0; i < threads; i++)
    { /* Wait for all the threads to terminate */
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            perror("Failed to join thread\n");
            exit(EXIT_FAILURE);
        }
    }
    print(votes); // Print votes to the files
    /* Proper deallocations and file descriptor closing */
    dele(votes);
    del(voters);
    close(sock);
    deletes(&clients);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&buff);
    free(thread_id);
    fclose(fdlog);
    fclose(fdstats);
    exit(0);
}
int main(int argc, char **argv)
{
    if (argc != 6)
    {
        printf("Not enough arguments, try $poller [portnum] [numWorkerthreads] [bufferSize] [poll-log][poll-stats]\n");
        exit(-1);
    }
    int port = atoi(argv[1]), buffersize = atoi(argv[3]), newsock;
    char *logs = argv[4], *stats = argv[5], buf[1024];
    threads = atoi(argv[2]);
    fdlog = fopen(logs, "w");
    fdstats = fopen(stats, "w");
    if (fdlog == NULL || fdstats == NULL)
    { /* If an error comes up while opening a file print error */
        perror("open");
        return -1;
    }
    /* Proper initializations */
    voters = NULL;
    votes = NULL;
    flag = 0; // Message for threads to NOT terminate
    init(&clients, buffersize);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&buff, NULL);
    thread_id = malloc(threads * sizeof(pthread_t));

    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct sockaddr *clientptr = (struct sockaddr *)&client;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket", -1, NULL);

    server.sin_family = AF_INET; /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port); /* The given port */
    if (bind(sock, serverptr, sizeof(server)) < 0)
        perror_exit("bind", sock, NULL);
    if (listen(sock, 128) < 0)
        perror_exit("listen", sock, NULL);

    printf("Listening for connections to port %d\n", port);

    signal(SIGINT, SIG_IGN); // Ignore SIGINT in all threads
    for (long i = 0; i < threads; i++)
    {
        if (pthread_create(&thread_id[i], NULL, serve, (void *)i) != 0)
        {
            perror("Failed to create thread\n");
            return EXIT_FAILURE;
        }
    }
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL); // Change the handling of SIGINT ONLY for the main thread

    do
    { /* Repeat until a SIGINT signal is given by the user */
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
        { /* Accept a connection and store at newsock the file descriptor to communicate */
            perror("accept");
            sig_handler(0);
        }

        pthread_mutex_lock(&mutex);
        /* If the buffer is full, wait till some clients are served */
        while (clients.count == clients.buffersize)
            pthread_cond_wait(&buff, &mutex);
        add(&clients, newsock, clientptr); // Add client to the buffer
        pthread_mutex_unlock(&mutex);
    } while (1);
    return 0;
}
