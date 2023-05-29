#include "Interface.h"
#include "InterfaceServer.h"

void sig_handler(int sig)
{

    flag = 1;

    pthread_cond_broadcast(&cond);
    for (int i = 0; i < threads; i++)
    {
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            perror("Failed to join thread\n");
            exit(EXIT_FAILURE);
        }
    }
    print(votes);

    dele(votes);
    del(voters);
    close(sock);
    deletes(&clients);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&buff);
    free(thread_id);
    fclose(fdlog);
    fclose(fdstats);
    exit(0);
}
int main(int argc, char **argv)
{
    voters = NULL;
    votes = NULL;
    flag = 0;
    if (argc != 6)
    {
        printf("Not enough arguments, try poller [portnum] [numWorkerthreads] [bufferSize] [poll-log][poll-stats]\n");
        exit(-1);
    }
    int port = atoi(argv[1]), buffersize = atoi(argv[3]), newsock;
    char *logs = argv[4], *stats = argv[5], buf[1024];
    fdlog = fopen(logs, "w");
    fdstats = fopen(stats, "w");
    if (fdlog == NULL || fdstats == NULL)
    { /* If an error comes up while opening a file print error */
        perror("open");
        return -1;
    }
    threads = atoi(argv[2]);
    init(&clients, buffersize);
    // signal(SIGINT, sig_handler);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex1, NULL);
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

    for (long i = 0; i < threads; i++)
    {
        if (pthread_create(&thread_id[i], NULL, serve, (void *)i) != 0)
        {
            perror("Failed to create thread\n");
            return EXIT_FAILURE;
        }
    }
    signal(SIGINT, SIG_IGN); // Ignore SIGINT in all threads except the main thread
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL); // Register the signal handler in the main thread

    do
    {
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
        {
            perror("accept");
        }

        // pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex);
        while (clients.count == clients.buffersize)
            pthread_cond_wait(&buff, &mutex);
        add(&clients, newsock, clientptr);
        pthread_mutex_unlock(&mutex);
        // pthread_mutex_unlock(&mutex1);
    } while (1);
    /* for (int i = 0; i < threads; i++)
    {
        if (pthread_join(thread_id[i], NULL) != 0)
        {
            perror("Failed to join thread\n");
            return EXIT_FAILURE;
        }
    }
    close(sock);
    free(thread_id);
    deletes(&clients);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);*/
    return 0;
}
