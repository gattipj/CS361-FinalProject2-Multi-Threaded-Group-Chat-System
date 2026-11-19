#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// DO NOT MODIFY THIS FUNCTION!!!

char *
get_port (void)
{
  char *port_str = malloc (12 * sizeof (char));
  if (port_str == NULL)
    return NULL; // Handle allocation failure

  uid_t uid = getuid ();
  // Port is (student's UID mod 37173) + 3717, keeping it in 3717..40889
  // so large UIDs (e.g. 50,000+) cannot overflow the 16-bit port range
  unsigned int port_num = 3717 + ((unsigned int)uid % 37173u);

  // Convert the integer to a string
  snprintf (port_str, 12, "%u", port_num);

  return port_str;
}
