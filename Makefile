# Compiler
CC = gcc

# .c files
Files1 = poller.c Functions.c
Files2 = pollerclient.c Functions.c

# .o objects
OBJS1 = poller.o Functions.o
OBJS2 = pollerclient.o Functions.o

# Executable
EXEC1 = poller
EXEC2 = pollerclient

# Create executable
$(EXEC1): $(OBJS1) $(Files1)
	$(CC) -g -Wall $(OBJS1) -o $(EXEC1) -lpthread

$(EXEC2): $(OBJS2) $(Files2)
	$(CC) -g -Wall $(OBJS2) -o $(EXEC2) -lpthread

# Clear
clean:
	rm -f $(EXEC1) $(OBJS1)
	rm -f $(EXEC2) $(OBJS2)
