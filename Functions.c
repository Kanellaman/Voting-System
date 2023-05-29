#include "Interface.h"
#include "InterfaceServer.h"
#include "InterfaceClient.h"
void *serve(void *arg)
{
    long i = (long)arg;
    printf("Hello from%ld\n", i);
    while (1)
    {
        char name[1024], party[1024];
        pthread_mutex_lock(&mutex);
        while (clients.count == 0 && !flag)
            pthread_cond_wait(&cond, &mutex);
        if (flag)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        printf("%d-By %ld\n", clients.count, i);
        req client = pop(&clients);
        pthread_mutex_unlock(&mutex);
        char response[17] = "SEND NAME PLEASE";

        if (write(client->socket, response, strlen(response) + 1) < 0)
            perror_exit("write", client->socket, client);

        if (read(client->socket, name, sizeof(name)) < 0)
            perror_exit("read", client->socket, client);

        pthread_mutex_lock(&mutex1);
        int found = search(voters, name);
        if (found)
            strcpy(response, "ALREADY VOTED");
        else
        {
            voters = insert(voters, name);
            strcpy(response, "SEND VOTE PLEASE");
        }
        if (write(client->socket, response, strlen(response) + 1) < 0)
            perror_exit("write", client->socket, client);

        if (!found)
        {
            if (read(client->socket, party, sizeof(party)) < 0)
                perror_exit("read", client->socket, client);

            votes = in(votes, name, party);
            strcpy(response, "VOTE for Party ");
            strcat(response, party);
            strcat(response, " RECORDED\0");
            if (write(client->socket, response, strlen(response) + 1) < 0)
                perror_exit("write", client->socket, client);
        }
        pthread_mutex_unlock(&mutex1);
        close(client->socket);
        free(client);
    }
    return NULL;
}

/* Wait for all dead child processes */
void perror_exit(char *message, int socket, void *dealloc)
{
    if (dealloc != NULL)
        free(dealloc);
    if (socket > 0)
        close(socket);
    perror(message);
    exit(EXIT_FAILURE);
}

void init(waits clients, int buffersize)
{
    clients->count = 0;
    clients->buffersize = buffersize;
    clients->head = NULL;
    clients->tail = NULL;
}

void add(waits clients, int socket, struct sockaddr *client)
{
    if (clients->count + 1 > clients->buffersize)
    {
        printf("Threw one");
        return;
    }
    req new = malloc(sizeof(struct Request));
    new->socket = socket;
    new->next = NULL;
    if (clients->head == NULL)
    {
        clients->head = new;
        clients->tail = new;
    }
    else
    {
        clients->tail->next = new;
        clients->tail = new;
    }
    pthread_cond_signal(&cond);
    clients->count++;
}

void deletes(waits clients)
{
    if (clients == NULL)
        return;

    req current = clients->head, next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    clients->head = NULL;
    clients->tail = NULL;
    clients->count = 0;
}

req pop(waits clients)
{
    req poped = clients->head;
    if (clients->count == 0)
        return NULL;
    if (poped->next == NULL)
        clients->tail = NULL;
    clients->count--;
    if (clients->count + 1 == clients->buffersize)
        pthread_cond_signal(&buff);
    clients->head = poped->next;

    return poped;
}
int search(name voters, char *voter)
{
    if (voters == NULL)
        return 0;
    while (voters != NULL)
    {
        if (!strcmp(voters->voter, voter))
            return 1;
        voters = voters->next;
    }
    return 0;
}
void del(name voters)
{
    if (voters == NULL)
        return;
    name current = voters, next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current->voter);
        free(current);
        current = next;
    }
}
name insert(name voters, char *voter)
{
    name new = malloc(sizeof(struct names));
    new->voter = malloc(strlen(voter) * sizeof(char) + 1);
    strcpy(new->voter, voter);
    new->next = NULL;
    if (voters != NULL)
        new->next = voters;
    return new;
}
void print(parties votes)
{
    while (votes != NULL)
    {
        if (votes->count != -1)
            fprintf(fdstats, "%s  %d\n", votes->party, votes->count);
        fprintf(fdlog, "%s  %s\n", votes->name, votes->party);

        votes = votes->next;
    }
}
parties in(parties votes, char *name, char *party)
{
    parties new = malloc(sizeof(struct Party)), prev, tmp = votes, last;
    int found = 0;
    new->name = malloc(strlen(name) * sizeof(char) + 1);
    strcpy(new->name, name);
    new->next = NULL;
    new->count = -1;
    if (votes == NULL)
    {
        new->count = 1;
        new->party = malloc(strlen(party) * sizeof(char) + 1);
        strcpy(new->party, party);
        return new;
    }
    last = votes;
    while (tmp != NULL && tmp->count != -1)
    {
        prev = tmp;
        if (!strcmp(tmp->party, party))
        {
            found = 1;
            tmp->count++;
            last = tmp;
        }
        tmp = tmp->next;
    }
    new->next = prev->next;
    prev->next = new;
    if (!found)
    {
        new->count = 1;
        new->party = malloc(strlen(party) * sizeof(char) + 1);
        strcpy(new->party, party);
    }
    else
    {
        new->party = last->party;
    }
    return votes;
}

void dele(parties votes)
{
    parties current = votes, next;
    while (current != NULL)
    {
        next = current->next;
        free(current->name);
        if (current->count != -1)
            free(current->party);
        free(current);
        current = next;
    }
}