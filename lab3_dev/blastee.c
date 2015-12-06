/* Author: Xuyi Ruan, Yunhe Liu
 * Data: Dec 1, 2015
 * Acknowledgement: Some code in this file are
 * referenced from Linux Programmer's Manual (the Man Page)
 * and stackoverflow, and wikipedia
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  //valid input command
  if (argc != 5)
  {
    error("Blastee command line argument number error\n");
    exit(1);
  }

  int blastee_socket;
  struct sockaddr_in sa;
  char buffer[200];
  ssize_t recsize;
  socklen_t fromlen;

  /* create an UDP socket*/
  if(-1 == (blastee_socket = socket(AF_INET, SOCK_DGRAM, 0)))
  {
    error("Socket() failed\n");
  }

  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  //TODO what is the INADDR_ANY here
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  sa.sin_port = htons(atoi(argv[3]));
  fromlen = sizeof(sa);

  if(-1 == bind(blastee_socket, (struct sockaddr *) &sa, fromlen))
  {
     error("bind() failed\n");
  }

  for (;;)
  {
    recsize = recvfrom(blastee_socket, (void*)buffer, sizeof buffer, 0, (struct sockaddr*)&sa, &fromlen);

    if(recsize < 0)
    {
      error("error: recsive < 0");
    }
    printf("datagram: %.*s\n", (int)recsize, buffer);
  }

  //TODO: return 0 for now, modify later
  return 0;
}
