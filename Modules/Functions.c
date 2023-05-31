#include "../include/Interface.h"
#include "../include/InterfaceServer.h"
#include "../include/InterfaceClient.h"

void *serve(void *arg)
{ /* Function for server-threads */                                                                                                                                                             
    while (1)
    {
        char name[1024], party[1024]; // Variables to store voter and party
        pthread_mutex_lock(&mutex);
        while (clients.count == 0 && !flag) // Wait until there is someone to serve or you need to terminate imidiately(flag)
            pthread_cond_wait(&cond, &mutex);
        if (flag)
        { /* Terminate-Command from sig_handler */
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        req client = pop(&clients); // Get a client to serve
        pthread_mutex_unlock(&mutex);
        char response[17] = "SEND NAME PLEASE";
        /* Send confirmation message */
        if (write(client->socket, response, strlen(response) + 1) < 0)
            return perror_exit("write", client->socket, client);
        /* Receive voter name */
        if (read(client->socket, name, sizeof(name)) < 0)
            return perror_exit("read", client->socket, client);

        pthread_mutex_lock(&mutex1);
        int found = search(voters, name);
        if (found)
            strcpy(response, "ALREADY VOTED");
        else
        {
            voters = insert(voters, name);
            strcpy(response, "SEND VOTE PLEASE");
        }
        pthread_mutex_unlock(&mutex1);
        /* Send message for confirmation to proceed or decline of vote */
        if (write(client->socket, response, strlen(response) + 1) < 0)
            return perror_exit("write", client->socket, client);

        if (!found)
        { /* Receive vote party if the voter has not voted again */
            if (read(client->socket, party, sizeof(party)) < 0)
                return perror_exit("read", client->socket, client);
            pthread_mutex_lock(&mutex2);
            votes = in(votes, party);
            fprintf(fdlog, "%s %s\n", name, party);

            pthread_mutex_unlock(&mutex2);
            strcpy(response, "VOTE for Party ");
            strcat(response, party);
            strcat(response, " RECORDED\0");
            /* Send confirmation vote */
            if (write(client->socket, response, strlen(response) + 1) < 0)
                return perror_exit("write", client->socket, client);
        }
        close(client->socket); // Close file descriptor
        free(client);          // Free served client
    }
    return NULL;
}

void *send_vote(void *line)
{
    char *line_text = (char *)line;
    int sock;
    char buf[1024] = "random";
    char *saveptr = NULL;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return perror_exit("socket", -1, line_text);
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(struct sockaddr_in)) < 0)
        return perror_exit("connect", sock, line_text);
    /* Read response of server */
    if (read(sock, buf, sizeof(buf)) == -1)
        return perror_exit("read", sock, line_text);
    if (strcmp(buf, "SEND NAME PLEASE"))
    { /* If not a confirmation message is received exit */
        close(sock);
        free(line_text);
        return NULL;
    }
    char *response = strtok_r(line_text, " ", &saveptr);
    char *rest = strtok_r(NULL, " ", &saveptr);
    char *name = malloc((strlen(response) + 1 + strlen(rest) + 1) * sizeof(char));
    /* build up the name as it has only one space between name and surname */
    strcpy(name, response);
    strcat(name, " ");
    strcat(name, rest);
    if (write(sock, name, strlen(name) + 1) == -1)
    { /* Send name and surname of voter */
        free(name);
        return perror_exit("write", sock, line_text);
    }
    free(name);
    /* Read response of server */
    if (read(sock, buf, sizeof(buf)) == -1)
        return perror_exit("read", sock, line_text);
    if (!strcmp(buf, "ALREADY VOTED"))
    { /* If the voter has already voted end */
        free(line_text);
        close(sock);
        return NULL;
    }
    response = strtok_r(NULL, " \n", &saveptr);
    /* Send the voted political party of voter */
    if (write(sock, response, strlen(response) + 1) == -1)
        return perror_exit("write", sock, line_text);
    /* Read response of server */
    if (read(sock, buf, sizeof(buf)) == -1)
        return perror_exit("read", sock, line_text);
    close(sock);     // Close socket
    free(line_text); // Free memory
    return NULL;
}

void *perror_exit(char *message, int socket, void *dealloc)
{ /* Free memory/close socket in case of an error and print message */
    if (dealloc != NULL)
        free(dealloc);
    if (socket > 0)
        close(socket);
    perror(message);
    return NULL;
}

void init(waits clients, int buffersize)
{ /* Initialize struct's contains */
    clients->count = 0;
    clients->buffersize = buffersize;
    clients->head = NULL;
    clients->tail = NULL;
}

void add(waits clients, int socket, struct sockaddr *client)
{ /* Add a client's socket to the end of the waiting list */
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

req pop(waits clients)
{ /* Pop from the list the client who came first */
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

void deletes(waits clients)
{ /* Deleting all clients waiting */
    if (clients == NULL)
        return;

    req current = clients->head, next = NULL;
    while (current != NULL)
    {
        next = current->next;
        close(current->socket);
        free(current);
        current = next;
    }
    clients->head = NULL;
    clients->tail = NULL;
    clients->count = 0;
}

name insert(name voters, char *voter)
{ /* Deleting all the names of the voters */
    name new = malloc(sizeof(struct names));
    new->voter = malloc(strlen(voter) * sizeof(char) + 1);
    strcpy(new->voter, voter);
    new->next = NULL;
    if (voters != NULL)
        new->next = voters;
    return new;
}

int search(name voters, char *voter)
{ /* Seach for a voter-returns 1 if found/0 if not found */
    while (voters != NULL)
    {
        if (!strcmp(voters->voter, voter))
            return 1;
        voters = voters->next;
    }
    return 0;
}

void del(name voters)
{ /* Deleting all voters */
    name current = voters, next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current->voter);
        free(current);
        current = next;
    }
}

parties in(parties votes, char *party)
{ /* Insert a party in the list/Count the votes of each party/Dont insert duplicate values for parties just increment the vote counter of the party */
    parties new, prev, tmp = votes;
    while (tmp != NULL)
    {
        prev = tmp;
        if (!strcmp(tmp->party, party))
        {
            tmp->count++;
            return votes;
        }
        tmp = tmp->next;
    }
    new = malloc(sizeof(struct Party));
    new->next = NULL;
    new->count = 1;
    new->party = malloc(strlen(party) * sizeof(char) + 1);
    strcpy(new->party, party);
    if (votes == NULL)
        return new;
    prev->next = new;
    return votes;
}

void print(parties votes)
{ /* Print the proper data to the output files */
    while (votes != NULL)
    {
        fprintf(fdstats, "%s %d\n", votes->party, votes->count);
        votes = votes->next;
    }
}

void dele(parties votes)
{ /* Delete all votes */
    parties current = votes, next;
    while (current != NULL)
    {
        next = current->next;
        if (current->count != -1)
            free(current->party);
        free(current);
        current = next;
    }
}