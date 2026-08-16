#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 8192

void send_response(int client_fd, const char *status, const char *headers, const char *body) {
  char response[4096];
  int length = snprintf(response, sizeof(response), "HTTP/1.1 %s\r\n%sContent-Length: %zu\r\nConnection: close\r\n\r\n%s", status, headers, strlen(body), body);
  send(client_fd, response, (size_t)length, 0);
}

void decode_urlencoded(char *value) {
  char *read = value;
  char *write = value;
  while (*read != '\0') {
    if (*read == '+') {
      *write++ = ' ';
      read++;
    } else if (*read == '%' && isxdigit((unsigned char)read[1]) && isxdigit((unsigned char)read[2])) {
      unsigned int hex_value;
      sscanf(read + 1, "%2x", &hex_value);
      *write++ = (char)hex_value;
      read += 3;
    } else {
      *write++ = *read++;
    }
  }
  *write = '\0';
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
    char request[BUFFER_SIZE] = {0};
    if (client_fd < 0) continue;
    recv(client_fd, request, sizeof(request) - 1, 0);

    if (strncmp(request, "POST /message ", 14) == 0) {
      char *body = strstr(request, "\r\n\r\n");
      if (body != NULL) {
        body += 4;
        char *message = strstr(body, "message=");
        if (message != NULL) {
          message += strlen("message=");
          char *separator = strchr(message, '&');
          if (separator != NULL) *separator = '\0';
          decode_urlencoded(message);
          FILE *message_file = fopen("message.txt", "w");
          if (message_file != NULL) {
            fputs(message, message_file);
            fclose(message_file);
          }
        }
      }
      send_response(client_fd, "302 Found", "Location: /\r\n", "");
    } else if (strncmp(request, "GET / ", 6) == 0) {
      send_response(client_fd, "200 OK", "Content-Type: text/html; charset=utf-8\r\n", "<html><head><title>My Web Server</title></head><body><form action=\"/message\" method=\"POST\"><input type=\"text\" name=\"message\"><button type=\"submit\">Submit</button></form></body></html>");
    } else {
      send_response(client_fd, "200 OK", "Content-Type: text/html; charset=utf-8\r\n", "<html><head><title>My Web Server</title></head><body><h1>Hello, World!</h1></body></html>");
    }
    close(client_fd);
  }
}