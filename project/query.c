#include "dns.h"
#include "main.h"
#include "server.h"

void receive_messages(server_configuration *server_configuration, int sockfd)
{
	int send_byte = 0;

	struct sockaddr_in client_address;
	socklen_t address_length = sizeof(client_address);

	while (1)
	{
		char buffer[DNS_BUFFER_SIZE];
		bzero(&buffer, sizeof(buffer));

        puts("Waiting for request...");
		int recieve_byte = recvfrom(sockfd, buffer, DNS_BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &address_length);
		if (recieve_byte == -1)
		{
			exit_with_error("can`t receive data on a socket");
		}
        else
        {
            char* requested_address = get_requested_address(buffer);
            printf("Received query, requested address = %s, handling it now\n", requested_address);
            free(requested_address);
        }

		int process_id = fork();
		if (process_id == -1)
		{
			exit_with_error("can`t create process");
		}

		if (process_id == 0)
		{
			char *request = buffer;
            puts("Ready to handle request in a child process");
			// we're inside a fork process and can proceed to handle the actual logic
			if (is_url_in_blacklist(request, server_configuration))
			{
                puts("Refusing blacklisted call...");
				refused_answer(sockfd, request, recieve_byte, &client_address);
			}
			else
			{
				int size = resend_query(server_configuration, request, recieve_byte);
				send_byte = sendto(sockfd, request, size, 0, (struct sockaddr *)&client_address, address_length);
			}
			if (send_byte == -1)
			{
				exit_with_error("can`t sendto data to socket");
			}

			close(sockfd);
			exit(0);
		}
	}
}

/*
** resend datagram to master dns server.
** get datagram with valid value.
** resend datagram to client
*/

int resend_query(server_configuration *server, char *buffer, int recive_byte)
{
	int sockfd;
	struct sockaddr_in dest;
	socklen_t dest_adrs_len;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		exit_with_error("resend_query: can`t open socket");
	}

	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(server->dns_ip);
	dest.sin_port = htons(53);
	dest_adrs_len = sizeof(dest);

	if (sendto(sockfd, buffer, recive_byte, 0, (struct sockaddr *)&dest, sizeof(dest)) == -1)
	{
		exit_with_error("can`t sendto data to socket");
	}

	recive_byte = recvfrom(sockfd, buffer, DNS_BUFFER_SIZE, 0, (struct sockaddr *)&dest, &dest_adrs_len);
	if (recive_byte == -1)
	{
		exit_with_error("can`t receives data on a socket");
	}
	if (close(sockfd) == -1)
	{
		exit_with_error("can`t close socket");
	}

	return (recive_byte);
}
