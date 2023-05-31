<div># Assignment 2 - Server(poller)-Client(pollSwayer) application for vote counting</div>

Author: sdi2000064 - Konstantinos Kanellakis

## Summary
This project implements a server-client application for conducting a pre-election poll. The server, called poller, listens for client connections, receives votes, and stores them in a log file. The client, called pollSwayer, reads a list of voter names and votes from an input file and sends them to the server.

<div id="header" align="center">
  <img src="https://i.pinimg.com/originals/0e/4a/c3/0e4ac37acbff81cd087aa19692a07a9d.gif" width="100"/>
</div>

## How to Run the Application

1. **Compilation/Execution:**
   - Compile the server (poller) and client (pollSwayer) applications using the following command:
     ```
     make all
     ```
   - Run the server (poller) with the following command:
     ```
     ./poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
     ```
   - Run the client (pollSwayer) with the following command:
     ```
     ./pollSwayer [serverName] [portNum] [inputFile.txt]
     ```

2. **Scripts:**
   - Use the following scripts for various tasks:
     - `create_input.sh`: Generates an input file with random voter names and votes.
       ```
       ./create_input.sh politicalParties.txt numLines
       ```
     - `tallyVotes.sh`: Counts the number of votes each party received from the input file.
       ```
       ./tallyVotes.sh inputFile
       ```
     - `processLogFile.sh`: Counts the number of votes for each party from the poll-log file.
       ```
       ./processLogFile.sh poll-log
       ```

3. **Checking Results:**
   - To check if the poller and tallyVotes results match, run the following commands:
     ```
     cmp pollerResultsFile tallyResultsFile
     sort -o poll-stats poll-stats
     cmp tallyResultsFile poll-stats
     ```

## Programming Design Choices - Server (poller)
- The server implementation utilizes multiple threads and synchronization mechanisms such as mutexes and condition variables.
- The main thread accepts client connections and places them in a buffer for worker threads to handle.
- Worker threads read requests from the clients, record votes, and update the log and stats files.
- The server ensures synchronization between the master thread and worker threads using condition variables and mutexes.

## Programming Design Choices - Client (pollSwayer)
- The client application creates a thread for each line in the input file.
- Each thread connects to the server and sends the voter's name and vote.
- The client continues to read lines from the input file until all votes are sent.

## Note
- The implementation does not have any memory leaks.
- The server runs until it receives a SIGINT (Ctrl+C) signal.
- The results are stored in the poll-stats file when the server terminates.

## Purpose
The purpose of this assignment is to familiarize you with system programming in a Unix environment. It involves threading and network communication, specifically implementing a multithreaded server and a client application.

## Problem 1: Multi-threaded network server
The server application, poller, runs a pre-election poll. It accepts connections from clients, receives votes, and stores them in log and stats files.

## Problem 2: Batch client for testing purposes
The client application, pollSwayer, reads voter names and votes from an input file and sends them to the server for recording.

## Problem 3: Bash scripts
Bash scripts are provided to assist in debugging the program and generating input files. These scripts include create_input.sh, tallyVotes.sh, and processLogFile.sh.

For more details on each problem and the implementation, please refer to the comments in the code files or send me a message.
