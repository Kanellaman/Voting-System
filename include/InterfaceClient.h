#ifndef INTERFACECLIENT_H
#define INTERFACECLIENT_H
extern int port;
extern char *votes_file;
extern struct sockaddr_in server;
extern struct sockaddr *serverptr;
extern struct hostent *rem;

/* Functions needed-Implemented in Functions.c */
void *send_vote(void *line);

#endif