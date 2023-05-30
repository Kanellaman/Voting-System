#include "Interface.h"
#include "InterfaceClient.h"

int main(int argc, char **argv)
{
    pthread_t *thread_id;
    if (argc != 4)
    {
        printf("Not enough arguments, try  pollSwayer [serverName] [portNum] [inputFile.txt]\n");
        exit(-1);
    }
    port = atoi(argv[2]);
    char line[1024];
    int i, num_lines = 0;
    stats = argv[3];
    FILE *file;
    file = fopen(stats, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        num_lines++;
    }
    serverptr = (struct sockaddr *)&server;
    if ((rem = gethostbyname(argv[1])) == NULL)
    {
        herror(" gethostbyname ");
        exit(1);
    }
    server.sin_family = AF_INET;   /* Internet domain */
    server.sin_port = htons(port); /* Server port */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
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
        char *text = malloc(strlen(line) * sizeof(char) + 1);
        strcpy(text, line);
        if (pthread_create(&thread_id[i], NULL, send_vote, text) != 0)
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

    return 0;
}