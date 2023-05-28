typedef struct names *name;
struct names
{
    char *voter;
    name next;
};

int port;
char *stats;
struct sockaddr_in server;
struct sockaddr *serverptr;
struct hostent *rem;

int search(name votes, char *voter);
name del(name votes);
name insert(name votes, char *voter);