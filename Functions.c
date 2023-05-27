#include "Interface.h"
#include "InterfaceServer.h"
void *serve(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (clients.count == 0)
            pthread_cond_wait(&cond, &mutex);
        if (flag == 1)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        req client = pop(&clients);
        if (client == NULL)
            continue;
        pthread_mutex_unlock(&mutex);
        const char *response = "SEND NAME PLEASE";

        if (write(client->socket, response, strlen(response) + 1) < 0)
        {
            perror("write");
            close(client->socket);
            return NULL;
        }
        close(client->socket);
        free(client);
    }
    return NULL;
}

/* Wait for all dead child processes */
void perror_exit(char *message)
{
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
    if (clients->count + 1 == clients->buffersize)
        return;
    req new = malloc(sizeof(struct Request));
    new->client = client;
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
    clients->head = poped->next;

    return poped;
}