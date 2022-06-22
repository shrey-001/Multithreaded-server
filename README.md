# Multithreaded Chat server

## Project Description
This project implements a simple Chatroom server, in which multiple clients can join the server and chat among themselves. To improve the responsiveness, utilization of cpu and better communication among clients, we have used the concept of multithreading, where each clients is spawned as a new thread and they can interact with the server accordingly.
## Getting Started
Make sure you are working on UNIX/Linux based operating system or WSL(Windows Subsystem for Linux), as mutex and thread library in c++ is present in UNIX/LINUX only.

Clone the repository in your directory.
```
git clone https://github.com/shrey-001/Multithreaded-server.git
```
Compile the server.cpp and client.cpp and make a executable file with name server and client respectively.
```
g++ server.cpp -pthread -o server
```
```
g++ client.cpp -pthread -o client
```
Run the server
```
./server
```
Run any number of clients you wish using this.
```
./client
```
You will be asked your name to register when you execute the ```./client```, enter your name and start messaging. 
## Internal Working
### Server side
* Socket is created using socket() function.
* To reuse the address and port Setsockopt() is used.
* To bind the socket with the defined port and address Bind() is used.
* After succesfully binding the socket with port, we put server in passive mode using listen() where it waits for the client to approach the server to make a connection.
* Then accept() call is used by a server to accept a connection request from a client
* Then a new thread is created and handle_connection() is passed to it, where it uses functions to display messages to all clients.

### Client Side
* Client side socket is created using socket().
* Then connect() system call connects the socket referred to by the client to the address specified by server_socket.
## Learnings
* **OS Concept** - Learned about process & thread, their differences and features.
* **Multithreading** - Learned about the concept of Multithreading and its working in UNIX/LINUX environment.
* **Mutex Lock** - Learned about Mutex lock which is used for Process Synchronisation.
* **Socket Programming** - learned how to create a client-server model using socket programming.
## References
[Thread functions in C/C++ - GeeksforGeeks](https://www.geeksforgeeks.org/thread-functions-in-c-c/) <br />
[Mutex lock for Linux Thread Synchronization - GeeksforGeeks](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/) <br />
[Socket Programming](https://www.geeksforgeeks.org/socket-programming-cc/) <br />
[Multithreading in C/C++](https://www.geeksforgeeks.org/handling-multiple-clients-on-server-with-multithreading-using-socket-programming-in-c-cpp/) <br />

