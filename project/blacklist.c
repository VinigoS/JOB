#include "dns.h"


int check_name_list(char *address, char blacklist[100][254])
{
  int i = 0;
  while (i < 100)
  {
    size_t blacklist_entry_length = strlen(blacklist[i]);
    if (blacklist_entry_length == 0)
    {
        break;
    }
    char *match = strstr(address, blacklist[i]);
    printf("Checking %s against %s, found %s\n", address, blacklist[i], match);
    if (match != NULL)
    {
      return (1);
    }
    ++i;
  }
  return (0);
}

char *get_requested_address(char *dns_query)
{
    char* address_string = malloc(255);
    char* header = &dns_query[13];
    size_t header_length = strlen(header);
    memcpy(address_string, header, header_length);
    address_string[header_length] = '\0';

    char *character_ptr = address_string;
    while (*character_ptr != '\0')
    {
      if (*character_ptr < 32 || *character_ptr > 126)
      {
        *character_ptr = '.';
      }
      character_ptr++;
    }

    return address_string;
}

int is_url_in_blacklist(char *dns_query, server_configuration *server_configuration)
{
  puts("skipping header datagram...");
  char* address_string = get_requested_address(dns_query);

  printf("Checking hostname against blacklist: %s \n", address_string);
  int result = check_name_list(address_string, server_configuration->blacklist);
  free(address_string);

  return result;
}
