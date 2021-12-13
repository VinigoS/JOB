#ifndef SERVER_H
#define SERVER_H

int bind_dns_server_socket(char *bind_address);
void refused_answer(int sockfd, char *buffer, int byte, struct sockaddr_in *client);

#endif
