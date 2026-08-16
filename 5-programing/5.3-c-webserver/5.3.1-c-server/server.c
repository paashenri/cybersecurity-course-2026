#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 8192

int main(void) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    return EXIT_FAILURE;
  }

  int reuse_address = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address));

  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 || listen(server_fd, 10) < 0) {
    perror("bind or listen");
    close(server_fd);
    return EXIT_FAILURE;
  }

  printf("Server is running on http://localhost:%d\n", PORT);

  while (1) {
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
      perror("accept");
      continue;
    }

    char buffer[BUFFER_SIZE] = {0};
    ssize_t received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
      printf("Received request:\n%s\n", buffer);
    }

    close(client_fd);
  }
}