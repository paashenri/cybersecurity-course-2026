#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000

int main(void) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int reuse_address = 1;
  struct sockaddr_in address = {0};

  if (server_fd < 0) {
    perror("socket");
    return EXIT_FAILURE;
  }

  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address));
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
    if (client_fd < 0) continue;

    const char response[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: 89\r\n"
        "Connection: close\r\n\r\n"
        "<html><head><title>My Web Server</title></head><body><h1>Hello, World!</h1></body></html>";
    send(client_fd, response, sizeof(response) - 1, 0);
    close(client_fd);
  }
}