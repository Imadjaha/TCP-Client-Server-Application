
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

// Server where the computer is reachable.
#define SERVER "127.0.0.1"
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
  struct sockaddr_in si_other;
  int tcp_socket;
  char buf[MAX_BUF_LEN];
  char message[MAX_BUF_LEN];
  struct pollfd fds[2];
  int nfds = 2;

  // Create socket and establish connection
  // TODO
  tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_socket == -1) {
    die("Error creating socket.");
  }
  // Create server address
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  si_other.sin_addr.s_addr = inet_addr(SERVER);

  // Connect to server
  if (connect(tcp_socket, (struct sockaddr *)&si_other, sizeof(si_other)) == -1) {
    die("Error connecting to server.");
  }

  // Set up poll array with standard input and the socket
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;
  fds[1].fd = tcp_socket;
  fds[1].events = POLLIN;

  while (true) {
    printf("Waiting for input: \n");
    fflush(stdout);

    // Wait for user input or data from the socket
    int rc = poll(fds, nfds, -1);
    if (rc == 0)
      die("Poll timeout");
    if (rc < 0)
      die("Poll error");

    // Process user input
    if (fds[0].revents == POLLIN) {
      // TODO
      // Read the message
      if (fgets(message, MAX_BUF_LEN, stdin) != NULL) {
        if (send(tcp_socket, message, strlen(message), 0) == -1) {
          die("Error sending message.");
        }
      }

    }
    // Process incoming data from the server
    else if (fds[1].revents == POLLIN) {
      // TODO
      // Receive the message
      int length = recv(tcp_socket, buf, MAX_BUF_LEN, 0);
      if (length == 0) {
        printf("Server has closed the connection!\n");
        // close(tcp_socket);
        // exit(EXIT_SUCCESS);
          break;
      } else if (length < 0) {
        die("Error receiving message.");
      }

      // Display the message
      buf[length] = '\0'; // Null-terminate the received data
      printf("Client says: %s\n", buf);
    }
  }

  // Close the socket that is no longer needed (from now on, it will no longer be available for the program!)
  printf("Closing client socket\n");
  close(tcp_socket);

  return EXIT_SUCCESS;
}
