# Compiler
CC = gcc

#.c files
Files = poller.c Functions.c

#.o objects
OBJS = poller.o Functions.o

#Executable
EXEC = poller

# Create executable
$(EXEC): $(OBJS) $(Files)
	$(CC) -g -Wall $(OBJS) -o $(EXEC) -lpthread

# Execute
run: $(EXEC)
	./$(EXEC)

# Clear
clean:
	rm -f $(EXEC) $(OBJS)