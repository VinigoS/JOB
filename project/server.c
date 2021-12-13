#include "dns.h"
#include "main.h"

int bind_dns_server_socket(char *bind_address)
{
  int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

  if (server_socket == -1)
  {
    exit_with_error("can`t create socket");
  }

  struct sockaddr_in server;
  bzero(&server, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(bind_address); // set the listen address of the dns proxy
  server.sin_port = htons(53);                      // set the listen port of the dns proxy

  int allow_flag = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &allow_flag, sizeof(int)) == -1)
  {
    close(server_socket);
    exit_with_error("can`t set socket option");
  }

  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
  {
    close(server_socket);
    exit_with_error("Failed to bind socket");
  }

  return (server_socket);
}

/*
** set DNS_HEADER->rcode to ERROR
** return datagram with ERROR opcode to client
*/

void refused_answer(int sockfd, char *buffer, int byte, struct sockaddr_in *client) // send refused
{
  dns_header *dns_h = (dns_header *)buffer;
  socklen_t address_length = sizeof(*client);
  int send_byte;

  dns_h->qr = 1;        // set querry
  dns_h->rcode = 5;      // The server refused to answer for the query
  dns_h->ans_count = 0; // set number of answer entries

  send_byte = sendto(sockfd, buffer, byte, 0, (struct sockaddr *)client, address_length); // The system calls  sendto are used to transmit a message to another socket.
  if (send_byte == -1)
  {
    exit_with_error("can`t sendto data to socket");
  }
}
