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

ARGS = 9002 4 8 log.txt stats.txt

# Create executable
$(EXEC1): $(OBJS1) $(Files1) $(EXEC2)
	$(CC) -g -Wall $(OBJS1) -o $(EXEC1) -lpthread

$(EXEC2): $(OBJS2) $(Files2)
	$(CC) -g -Wall $(OBJS2) -o $(EXEC2) -lpthread

poll: $(EXEC1)
	./poller $(ARGS)

# Clear
clean:
	rm -f $(EXEC1) $(OBJS1)
	rm -f $(EXEC2) $(OBJS2)
