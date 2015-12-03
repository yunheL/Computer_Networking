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
  int bytes_sent;
  char buffer[200];

  /* create an UDP socket*/
  if(-1 == blastee_socket = socket(AF_INET, SOCK_DGRAM, 0))
  {
    error("Socket() failed\n");
  }
  
  //zero out socket address
  memset(&sa, 0, sizeof sa);

  sa.sin_family = AF_INET;




}
