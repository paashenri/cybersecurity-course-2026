#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000

void send_html(int client_fd, const char *body) {
  char response[4096];
  int length = snprintf(response, sizeof(response),
      "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n%s",
      strlen(body), body);
  send(client_fd, response, (size_t)length, 0);
}

int main(void) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int reuse_address = 1;
  struct sockaddr_in address = {0};

  if (server_fd < 0) return EXIT_FAILURE;
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
    char request[2048] = {0};
    if (client_fd < 0) continue;
    recv(client_fd, request, sizeof(request) - 1, 0);

    if (strncmp(request, "GET / ", 6) == 0) {
      send_html(client_fd, "<html><head><title>My Web Server</title></head><body><form action=\"/message\" method=\"POST\"><input type=\"text\" name=\"message\"><button type=\"submit\">Submit</button></form></body></html>");
    } else {
      send_html(client_fd, "<html><head><title>My Web Server</title></head><body><h1>Hello, World!</h1></body></html>");
    }
    close(client_fd);
  }
}