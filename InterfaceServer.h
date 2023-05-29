pthread_mutex_t mutex;
pthread_cond_t buff;
pthread_cond_t cond;
pthread_t *thread_id;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

int sock, threads, flag;
FILE *fdlog, *fdstats;

typedef struct Party *parties;
struct Party
{
    char *party;
    char *name;
    int count;
    parties next;
};

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

name voters;
parties votes;
/* Server */
int search(name votes, char *voter);
void del(name votes);
name insert(name votes, char *voter);
void *serve(void *rank);
void init(waits clients, int buffersize);
void add(waits clients, int socket, struct sockaddr *client);
void deletes(waits clients);
req pop(waits clients);
parties in(parties votes, char *name, char *party);
void print(parties votes);
void dele(parties votes);
