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
        int found = search(votes, name);
        if (found)
            strcpy(response, "ALREADY VOTED");
        else
        {
            votes = insert(votes, name);
            strcpy(response, "SEND VOTE PLEASE");
        }
        pthread_mutex_unlock(&mutex1);
        if (write(client->socket, response, strlen(response) + 1) < 0)
            perror_exit("write", client->socket, client);

        if (!found)
        {
            if (read(client->socket, party, sizeof(party)) < 0)
                perror_exit("read", client->socket, client);

            strcpy(response, "VOTE for Party ");
            strcat(response, party);
            strcat(response, " RECORDED\0");

            if (write(client->socket, response, strlen(response) + 1) < 0)
                perror_exit("write", client->socket, client);
        }
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
int search(name votes, char *voter)
{
    if (votes == NULL)
        return 0;
    while (votes != NULL)
    {
        if (!strcmp(votes->voter, voter))
            return 1;
        votes = votes->next;
    }
    return 0;
}
name del(name votes)
{
    if (votes == NULL)
        return NULL;
    name current = votes, next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current->voter);
        free(current);
        current = next;
    }
    return NULL;
}
name insert(name votes, char *voter)
{
    name new = malloc(sizeof(struct names));
    new->voter = malloc(strlen(voter) * sizeof(char) + 1);
    strcpy(new->voter, voter);
    new->next = NULL;
    if (votes != NULL)
        new->next = votes;
    return new;
}