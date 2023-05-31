Assignment 2 - Server(poller)-Client(pollSwayer) application for vote counting
sdi2000064-Konstantinos Kanellakis

Compilation/Execute

!First run create_input.sh as show below to create the input file for pollSwayer!

For compilation use command:
    make all

For executing use command:
    ./poller 9000 8 16 poll-log poll-stats
    ./pollSwayer linux01.di.uoa.gr 9000 inputFile
Scripts
    ./create_input.sh politicalParties.txt numLines
    ./tallyVotes.sh inputFile
    ./processLogFile.sh poll-log

To check if the results are correct run:
    cmp pollerResultsFile tallyResultsFile
    sort -o poll-stats poll-stats
    cmp tallyResultsFile poll-stats

Programming Design Choices-Server(poller)

The sig_handler function is a signal handler for the SIGINT signal (Ctrl+C). It sets a flag to indicate that the threads should terminate and performs cleanup tasks 
such as freeing memory, printing results, closing file descriptors, and destroying mutexes and condition variables.
It is important to note that only the main thread receivs the SIGINT signal and handles it and NOT the threads.

There has been used 3 mutexes and 2 condition variables.
The purpose of condition variable buff is to prevent main thread accept any more requests while the buffer is full.
The other condition variable cond is to prevent each worker thread to try and connect to a client when there is no one waiting, when the buffer is empty.
Both condtiion variables are used in conjuction with the same mutex.

The other two mutexes are preventing the race conditions in searching/printing voters' names and vote counting. One mutex could be used for both operations
but I believe that with two, the synchronization and scheduling is better as a thread is not holding too long a mutex for unecessary operations.

Server imlements the buffer with a priority queue (FIFO). The first requests that comes is served first. To implement this we are using 2 structs. One struct, for storing
the first element and the last element of the list and the number of requests pending,and an another struct that represents the request.
Server also uses a list to store the names of the voters and check if a voter has voted again
A list has also been used to store the political parties and the votes of each one of them

Programming Design Choices-Server(pollerSwayer)

For every non empty line of the input file a thread is created. 
Each thread is assigned 1 line(vote) of the file and is responsible for it to be delivered to the poller.

If all threads have delivered their vote the program ends.

Note:
There are no memory leaks!
The server runs until it receives a SIGINT (Ctrl+C) signal.