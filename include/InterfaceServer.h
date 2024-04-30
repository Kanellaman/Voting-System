#ifndef INTERFACESERVER_H
#define INTERFACESERVER_H

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
{ /* Store the number of votes so that are printed at the proper output file*/
    char *party;
    int count;
    parties next;
};

/* Mutexes/Condtion Variables/Thread variable */
extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern pthread_cond_t buff;
extern pthread_cond_t cond;
extern  pthread_t *thread_id;

extern int sock, threads, flag;
extern FILE *fdlog, *fdstats; // Input-Output files

/* Ckeck typedefs and structs' comments */
extern struct waiting clients;
extern name voters;
extern parties votes;

/* Functions needed-Implemented in Functions.c */
void *serve(void *rank);

void init(waits clients, int buffersize);
void add(waits clients, int socket, struct sockaddr *client);
void deletes(waits clients);
req pop(waits clients);

name insert(name votes, char *voter);
int search(name votes, char *voter);
void del(name votes);

parties in(parties votes, char *party);
void print(parties votes);
void dele(parties votes);

#endif