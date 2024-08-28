#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

// Length of the queue (maximum number of connection requests from clients)
#define BACKLOG_SIZE 1
#define MAX_BUF_LEN 512
/* The port on which the socket should listen / bind.
 * You need to adjust this as required in the assignment,
 * i.e., replace the xxxx with the last four digits of your matriculation number!
 */
#define PORT 55555

// Small helper function for displaying error messages
void die(char *error_msg) {
  perror(error_msg);
  exit(EXIT_FAILURE);
}

int main(void) {
  struct sockaddr_in si[100];
  int tcp_socket[100];
  char buf[MAX_BUF_LEN];
  socklen_t si_addr[100];
  struct pollfd fds[100];
  int nfds = 1;

  // Create socket
  // TODO
  tcp_socket[0] = socket(AF_INET, SOCK_STREAM, 0);

  if (tcp_socket[0] < 0) {
    die("Error creating socket.");
  }
  printf("Socket was successfully created.\n");

  // Set the SO_REUSEADDR option, which allows immediate reuse of the socket after closing
  setsockopt(tcp_socket[0], SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

  // We need to initialize the structure for our "incoming" socket with zeros
  memset((char *)&si[0], 0, sizeof(si[0]));
  // Then we can enter the desired values
  si[0].sin_family = AF_INET;
  si[0].sin_port = htons(PORT);
  si[0].sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind the socket to a port to be able to receive messages
  // TODO
  // Here the socket is bound to the address and port specified in the structure si[0].
  int bind_status = bind(tcp_socket[0], (struct sockaddr *)&si[0], sizeof(si[0]));
  if (bind_status < 0) {
    die("Error binding socket.");
  }

  // Prepare to "listen" for client connection requests
  //
  // Here the socket is put into a state where it can accept incoming connections.
  if (listen(tcp_socket[0], BACKLOG_SIZE) < 0) {
    die("Error listening on socket.");
  } else {
    printf("Socket is successfully listening.\n");
  }

  // Initialize the poll array
  memset(fds, 0, sizeof(fds));

  // The server socket is added to the poll array
  fds[0].fd = tcp_socket[0];
  fds[0].events = POLLIN;

  while (true) {
    printf("Waiting for data...\n");
    printf("%d clients connected\n", nfds - 1);
    fflush(stdout);

    // Wait for incoming data on any connected socket
    int rc = poll(fds, nfds, -1);
    if (rc == 0)
      die("Poll timeout");
    if (rc < 0)
      die("Poll error");

    // New incoming connection
    if (fds[0].revents == POLLIN) {
      // TODO
      // Accept the new connection
      si_addr[nfds] = sizeof(si[nfds]);
      tcp_socket[nfds] =
          accept(tcp_socket[0], (struct sockaddr *)&si[nfds], &si_addr[nfds]);
      if (tcp_socket[nfds] < 0) {
        die("Error accepting connection.");
      }

      // Add the new socket to the poll array
      fds[nfds].fd = tcp_socket[nfds];
      fds[nfds].events = POLLIN;       
      nfds++;
    }

    // Check existing connections for incoming data
    for (int i = 1; i < nfds; i++) {
      if (fds[i].revents == POLLIN) {
        // TODO
        // Receive data from client number i
        int length = recv(fds[i].fd, buf, MAX_BUF_LEN, 0);
        if (length <= 0) {
          // Connection was closed or error receiving
          printf("Client %d has closed the connection.\n", i);
          close(fds[i].fd);
          fds[i].fd = -1;
          nfds--; // reduce the number of connected clients
          goto End;
            
        } else {
          // Process the received data (e.g., echo)
          buf[length] = '\0'; // Add null-terminator
          printf("Client %d says: %s\n", i, buf);
          // Send message from client i to all other clients
          for (int j = 1; j < nfds; j++) {
            if (j != i) {
              int messageLen = send(fds[j].fd, buf, length, 0); 
              if (messageLen < 0) {
                die("Error sending message.");
              }
            }
          }
           
        }
      }
    }
  }

  // Closing all connections
  End:
  printf("Closing server\n");
  for (int i = 0; i < nfds; i++) {
    printf("Closing connection %d\n", i);
    close(tcp_socket[i]);
  }

  return EXIT_SUCCESS;
}
