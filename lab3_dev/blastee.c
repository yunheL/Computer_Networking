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
  printf("here!\n");
  //valid input command
  if (argc != 5)
  {
    error("Blastee command line argument number error\n");
    exit(1);
  }
  printf("argv[2] is: %s + argv[4]: %s\n", argv[2], argv[4]);

  int blastee_socket;
  struct sockaddr_in sa;
  char buffer[1000];
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
  sa.sin_addr.s_addr = inet_addr("128.105.37.194");
  //sa.sin_addr.s_addr = htonl(INADDR_ANY);
  
  printf("binding to address: %s", inet_ntoa(sa.sin_addr));
  sa.sin_port = htons(atoi(argv[2]));
  fromlen = sizeof(sa);

  if(-1 == bind(blastee_socket, (struct sockaddr *) &sa, fromlen))
  {
     error("bind() failed\n");
  }

/*
  if(-1 == listen(blastee_socket, 2))
  {
     error("listen() failed");
  }
*/

/*
  int acceptfd;
  acceptfd = (blastee_socket, (struct sockaddr *) &sa, fromlen)
  if(acceptfd < 0)
  {
     error("accept() failed");
  }

  printf("The accepted socket is %d", accetpfd);
*/
  printf("here????????\n");
  for (;;)
  {
    printf("Here4!!!!!!!"); 
    fflush(stdout);
    recsize = recvfrom(blastee_socket, (void*)buffer, sizeof buffer, 0, (struct sockaddr*)&sa, &fromlen);
    printf("Here3");
    if(recsize < 0)
    {
      error("error: recsive < 0");
    }
    //printf("datagram: %.*s\n", (int)recsize, buffer);
    printf("datagram: %s\n", buffer);
  }

  close(blastee_socket);
  //TODO: return 0 for now, modify later
  return 0;
}
