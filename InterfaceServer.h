/* Server header file */
typedef struct Request *req;
typedef struct waiting *waits;
typedef struct names *name;
typedef struct Party *parties;

struct Request
{ /* A struct made to store the socket of a client waiting to be served  */
    int socket;
    req next;
};
struct waiting
{ /* (Client buffer)A struct made to point to all every socket of the clients that have not yet been served */
    int count, buffersize;
    req head;
    req tail;
};
struct names
{ /* A struct to store all the names of the voters that have already voted */
    char *voter;
    name next;
};

struct Party
{ /* Store the number of votes and the voters so that are printed at the proper output files*/
    char *party;
    char *name;
    int count;
    parties next;
};

/* Mutexes/Condtion Variables/Thread variable */
pthread_mutex_t mutex;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_cond_t buff;
pthread_cond_t cond;
pthread_t *thread_id;

int sock, threads, flag;
FILE *fdlog, *fdstats; // Input-Output files

/* See typedefs and structs' comments */
struct waiting clients;
name voters;
parties votes;

/* Functions needed-Implemented in Functions.c */
void *serve(void *rank);

void init(waits clients, int buffersize);
void add(waits clients, int socket, struct sockaddr *client);
void deletes(waits clients);
req pop(waits clients);

name insert(name votes, char *voter);
int search(name votes, char *voter);
void del(name votes);

parties in(parties votes, char *name, char *party);
void print(parties votes);
void dele(parties votes);