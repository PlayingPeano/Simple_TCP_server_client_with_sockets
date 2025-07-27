#include <arpa/inet.h>
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

const char* LOCALHOST = "127.0.0.1";
const char* OK_MESSAGE = "OK\n";
const int BUFFER_SIZE = 4;
const int PORT = 8080;

int
main(int argc, char* argv[])
{
  int client_socket{};
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Bad socket creation");
    return EXIT_FAILURE;
  }

  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  if (inet_pton(AF_INET, LOCALHOST, &server_address.sin_addr) < 0) {
    perror("Bad address");
    close(client_socket);
    return EXIT_FAILURE;
  }

  if (connect(client_socket,
              (sockaddr*)&server_address,
              sizeof(server_address)) < 0) {
    perror("Bad connection");
    close(client_socket);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE]{};
  ssize_t bytes_read{};
  if ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) < 0) {
    perror("Bad recv");
    close(client_socket);
    return EXIT_FAILURE;
  }
  if (strcmp(buffer, OK_MESSAGE) != 0) {
    perror("Bad response");
  }

  close(client_socket);
  return EXIT_SUCCESS;
}