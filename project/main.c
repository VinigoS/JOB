#include "server.h"
#include "dns.h"
#include "main.h"


void exit_with_error(char *error_text)
{
	perror(error_text);
	exit(1);
}

static void print_usage(char *binary_name)
{
	ft_putstr("Usage:\n");
	ft_putstr(binary_name);
	ft_putstr(" your_configuration_file.ini\n");
}

static void run_server(char *configuration_file_path)
{
	server_configuration server_configuration;
	init(&server_configuration);

	FILE *conf_fd;
	if ((conf_fd = fopen(configuration_file_path, "r")) == NULL) {
		exit_with_error(strcat("Failed to open configuration file: ", configuration_file_path));
	}

	read_conf_file(&server_configuration, conf_fd);

	int server_socket_fd = bind_dns_server_socket("0.0.0.0");
	receive_messages(&server_configuration, server_socket_fd);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		print_usage(argv[0]);
		return (1);
	}
	else
	{
        ft_putstr("DNS Proxy is running\n");
		run_server(argv [1]);
		return (0);
	}
}
