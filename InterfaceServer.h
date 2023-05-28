pthread_mutex_t mutex;
pthread_mutex_t mutex1;
pthread_cond_t buff;
pthread_cond_t cond;
pthread_t *thread_id;

int sock, threads, flag;
int fdlog, fdstats;

struct waiting clients;
typedef struct Request *req;
struct Request
{
    int socket;
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
