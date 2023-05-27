#include "Interface.h"
void *print_line(void *line)
{
    char *line_text = (char *)line;
    printf("%s", line_text);
    return NULL;
}
int main(int argc, char **argv)
{
    pthread_t *thread_id;
    if (argc != 4)
    {
        printf("Not enough arguments, try  pollSwayer [serverName] [portNum] [inputFile.txt]\n");
        exit(-1);
    }
    char line[1024];
    int port = atoi(argv[2]), sock, i, num_lines;
    char *name = argv[1], *stats = argv[3], buf[256];
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr *)&server;
    struct hostent *rem;
    FILE *file;
    file = fopen(stats, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strcmp(line, "\n"))
            num_lines++;
    }

    // Reset the file position indicator to the beginning
    fseek(file, 0, SEEK_SET);
    thread_id = (pthread_t *)malloc(num_lines * sizeof(pthread_t));
    if (thread_id == NULL)
    {
        printf("Error allocating memory for threads.\n");
        fclose(file);
        return 1;
    }
    for (i = 0; i < num_lines; i++)
    {
        fgets(line, sizeof(line), file);

        if (pthread_create(&thread_id[i], NULL, print_line, line) != 0)
        {
            printf("Error creating thread.\n");
            fclose(file);
            free(thread_id);
            return 1;
        }
    }
    for (i = 0; i < num_lines; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
    fclose(file);
    free(thread_id);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(-1);
    }
    if ((rem = gethostbyname(name)) == NULL)
    {
        herror(" gethostbyname ");
        exit(1);
    }
    server.sin_family = AF_INET;   /* Internet domain */
    server.sin_port = htons(port); /* Server port */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
    {
        perror_exit(" connect ");
        close(sock);
        exit(-1);
    }

    if (read(sock, buf, sizeof(buf)) <= 0)
    {
        printf("Error receiving message from server\n");
        close(sock);
        exit(-1);
    }
    if (strcmp(buf, "SEND NAME PLEASE"))
    {
        printf("Received wrong message from server %s", buf);
        close(sock);
        exit(-1);
    }
    printf("Ola good\n");
    close(sock);
    return 0;
}