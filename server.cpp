#include <assert.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const char* OK_MESSAGE = "OK\n";
const int PORT = 8080;
const int MAX_PENDING_CONNECTIONS_QUEUE_LENGTH = 1024;


int
main(int argc, char* argv[])
{
  int server_fd{};
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Bad socket creation");
    return EXIT_FAILURE;
  }

  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Bad bind");
    close(server_fd);
    return EXIT_FAILURE;
  }

  if (listen(server_fd, MAX_PENDING_CONNECTIONS_QUEUE_LENGTH) < 0) {
    perror("Bad listen");
    close(server_fd);
    return EXIT_FAILURE;
  }

  while (true) {
    sockaddr_in client_address{};
    socklen_t client_len = sizeof(client_address);

    int client_socket{};
    if ((client_socket =
           accept(server_fd, (sockaddr*)&client_address, &client_len)) < 0) {
      perror("Bad accept");
      continue;
    }

    ssize_t bytes_sent{};
    if ((bytes_sent = send(client_socket, OK_MESSAGE, strlen(OK_MESSAGE), 0)) !=
        strlen(OK_MESSAGE)) {
      perror("Bad send");
    }

    close(client_socket);
  }

  // No need?
  close(server_fd);
  return EXIT_SUCCESS;
}