int port;
char *votes_file;
struct sockaddr_in server;
struct sockaddr *serverptr;
struct hostent *rem;

/* Functions needed-Implemented in Functions.c */
void *send_vote(void *line);