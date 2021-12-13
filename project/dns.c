#include "dns.h"
#include "main.h"


static void strip_newline(char* line)
{
    line[strcspn(line, "\r\n")] = 0; //
}

static void read_blacklist(server_configuration *server_configuration, FILE *fd)
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read_bytes;
  int line_number = 0;

  while ((read_bytes = getline(&line, &len, fd)) != NULL)
  {
    strip_newline(line);
    if (strcmp(line, "") == 0)
    {
      break;
    }
    else
    {
      strcpy((server_configuration->blacklist)[line_number], line);
      line_number++;
    }
  }

  if (line)
  {
      free(line);
  }

}

static void read_nameserver(char *dns_ip, FILE *fd)
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read_bytes = getline(&line, &len, fd);

  if (read_bytes == NULL)
  {
    exit_with_error("Failed to read configuration file");
  }

  strip_newline(line);
  strcpy(dns_ip, line);
  free(line);
}

void init(server_configuration *server_configuration)
{
  bzero(server_configuration->dns_ip, sizeof(server_configuration->dns_ip));
  bzero(server_configuration->blacklist, sizeof(server_configuration->blacklist));
  server_configuration->name = NULL;
}

void read_conf_file(server_configuration *server_configuration, FILE *conf_fd)
{
  char *line = NULL;
  int read_result;
  size_t len = 0;

  ssize_t read;
  while((read = getline(&line, &len, conf_fd)) != -1)
  {
    strip_newline(line);
    if (!strcmp(line, "black_list:"))
    {
      read_blacklist(server_configuration, conf_fd);
    }
    else if (!strcmp(line, "nameserver:"))
    {
      read_nameserver(server_configuration->dns_ip, conf_fd);
    }
  }

  if (line)
  {
      free(line);
  }

  if (read_result == -1)
  {
    exit_with_error("Failed to read configuration file");
  }

  if (fclose(conf_fd) == -1)
  {
    exit_with_error("Failed to close configuration file");
  }
}
