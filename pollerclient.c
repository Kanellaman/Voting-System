#include "Interface.h"
#include "InterfaceClient.h"

void *print_line(void *line)
{
    char *line_text = (char *)line;
    int sock;
    char buf[1024];
    char *saveptr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket", -1, line_text);
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(struct sockaddr_in)) < 0)
        perror_exit("connect", sock, line_text);

    if (read(sock, buf, sizeof(buf)) < 0)
        perror_exit("read", sock, line_text);
    if (strcmp(buf, "SEND NAME PLEASE"))
    {
        printf("Received wrong message from server %s", buf);
        close(sock);
        free(line_text);
        return NULL;
    }
    char *response = strtok_r(line_text, " ", &saveptr);

    if (write(sock, response, strlen(response) + 1) < 0)
        perror_exit("write", sock, line_text);

    if (read(sock, buf, sizeof(buf)) < 0)
        perror_exit("read", sock, line_text);

    if (!strcmp(buf, "ALREADY VOTED"))
    {
        free(line_text);
        close(sock);
        return NULL;
    }
    response = strtok_r(NULL, " \n", &saveptr);
    printf("%s", response);
    if (write(sock, response, strlen(response) + 1) < 0)
        perror_exit("write", sock, line_text);
    if (read(sock, buf, sizeof(buf)) < 0)
        perror_exit("read", sock, line_text);
    close(sock);
    free(line_text);
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
        if (strcmp(line, "\n") != 0)
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
    printf("%d", num_lines);

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
        if (pthread_create(&thread_id[i], NULL, print_line, text) != 0)
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
    name test = NULL;
    test = insert(test, "Kostakhs");
    test = insert(test, "Liakos");
    test = insert(test, "Giorgos");
    int f = search(test, "Kostakhs");
    printf("Found? %d\n", f);
    f = search(test, "Kostak");
    printf("Found? %d\n", f);
    test = del(test);
    return 0;
}