pthread_mutex_t mutex;
pthread_cond_t buff;
pthread_cond_t cond;
pthread_t *thread_id;
pthread_mutex_t mutex1;

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
typedef struct names *name;
struct names
{
    char *voter;
    name next;
};

name votes;
/* Server */
int search(name votes, char *voter);
name del(name votes);
name insert(name votes, char *voter);
void *serve(void *rank);
void init(waits clients, int buffersize);
void add(waits clients, int socket, struct sockaddr *client);
void deletes(waits clients);
req pop(waits clients);
