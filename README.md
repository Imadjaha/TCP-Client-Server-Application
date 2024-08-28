# TCP-Client-Server-Application

## Overview

This project implements a simple TCP client-server application using C. The server accepts multiple client connections and broadcasts messages received from one client to all other connected clients. The implementation utilizes socket programming and `poll` to handle multiple client connections concurrently.

## Features

- **Multi-client support**: The server can handle multiple client connections simultaneously.
- **Message broadcasting**: Messages received from a client are broadcasted to all other connected clients.
- **Graceful error handling**: The application includes basic error handling to manage issues like connection failures and unexpected disconnections.
- **Cross-platform compatibility**: The code can be compiled and run on both Unix-like systems (Linux, macOS) and Windows with minor modifications.

## Getting Started

### Prerequisites

Ensure that you have the following tools installed on your system:

- **GCC**: A compiler to compile the C code.
- **Make** (optional): To simplify the build process.
- **Git**: For cloning the repository.

## Building the Application

### On Unix-like Systems (Linux/macOS)

You can compile the client and server programs using `gcc`:

```
gcc -o server server.c
gcc -o client client.c
```

Alternatively, you can use the Makefile (if provided):

```
make all
```
### On Windows
If you're using MinGW or another environment that supports gcc:

```
gcc -o server.exe server.c -lws2_32
gcc -o client.exe client.c -lws2_32
```

### Running The Applicaton
Start the Server
First, start the server:

```
./server
```
The server will begin listening on the specified port for incoming client connections.

Start the Client(s)
In a separate terminal or on another machine, start the client:

```
./client
```
You can start multiple clients to connect to the server. Each client can send messages that will be broadcasted to all other connected clients

### Port Configuration
You can modify the port number by editing the #define PORT line in both client.c and server.c to suit your environment.
