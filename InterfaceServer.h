struct waiting clients;
pthread_mutex_t mutex;
pthread_t *thread_id;
int sock,threads,flag;
pthread_cond_t cond;
typedef struct Request *req;
struct Request
{
    int socket;
    struct sockaddr *client;
    req next;
};
struct waiting
{
    int count, buffersize;
    req head;
    req tail;
};
typedef struct waiting *waits;
/* Server */

void *serve(void *rank);
void init(waits clients, int buffersize);
void add(waits clients, int socket, struct sockaddr *client);
void deletes(waits clients);
req pop(waits clients);
