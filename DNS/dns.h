#ifndef DNS_SERVER_H
#define DNS_SERVER_H

#include "../libft-master/includes/libft.h"
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DNS_BUFFER_SIZE 512

typedef struct
{
	char dns_ip[17];
	char blacklist[100][254];
	struct host *name;
} server_configuration;



typedef struct
{
	unsigned short id;				 // identification number            2 byte
	unsigned char rd : 1;			 // recursion desired
	unsigned char tc : 1;			 // truncated message
	unsigned char aa : 1;			 // authoritive answer
	unsigned char opcode : 4;	 // purpose of message
	unsigned char qr : 1;			 // query/response flag              1 byte
	unsigned char rcode : 4;	 // response code
	unsigned char z : 1;			 // its z! reserved
	unsigned char ra : 1;			 // recursion available              1 byte
	unsigned short q_count;		 // number of question entries       2 byte
	unsigned short ans_count;	 // number of answer entries         2 byte
	unsigned short auth_count; // number of authority entries      2 byte
	unsigned short add_count;	 // number of resource entries       2 byte
} dns_header;


void init(server_configuration *server_configuration);
void read_conf_file(server_configuration *server, FILE *fd);
void receive_messages(server_configuration *server_configuration, int sockfd);
char *get_requested_address(char *request);
int is_url_in_blacklist(char *buffer, server_configuration *server);
int resend_query(server_configuration *server, char *buffer, int recive_byte);

#endif
