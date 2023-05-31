# Compiler
CC = gcc

# .c files
Files1 = poller.c ../modules/Functions.c
Files2 = pollSwayer.c ../modules/Functions.c

# .o objects
OBJS1 = poller.o ../modules/Functions.o
OBJS2 = pollSwayer.o ../modules/Functions.o

# .h files
InterfaceServer = ../include/InterfaceServer.h ../include/Interface.h
InterfaceClient = ../include/InterfaceClient.h ../include/Interface.h

# Executable
EXEC1 = poller
EXEC2 = pollSwayer

ARGS1 = 9000 3 10 log stats
ARGS2 = localhost 9000 inputFile

# Create executable
$(EXEC1): $(OBJS1) $(Files1) $(InterfaceServer)
	$(CC) -g -Wall $(OBJS1) -o $(EXEC1) -lpthread

$(EXEC2): $(OBJS2) $(Files2) $(InterfaceClient)
	$(CC) -g -Wall $(OBJS2) -o $(EXEC2) -lpthread

# Make all files
all: $(EXEC1) $(EXEC2)

# Check for memory leaks
memory-poller: $(EXEC1)
	valgrind --leak-check=full --track-origins=yes ./$(EXEC1) $(ARGS1)

memory-pollSwayer: $(EXEC2)
	valgrind --leak-check=full --track-origins=yes ./$(EXEC2) $(ARGS2)

# Clear
clean:
	rm -f $(EXEC1) $(OBJS1) $(EXEC2) $(OBJS2) 
