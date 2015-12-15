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
#include <time.h>

//function to print out error msg
void error(const char *msg)
{
  perror(msg);
  exit(-1);
}

int main(int argc, char *argv[])
{
  //valid input command
  if (argc != 5)
  {
    error("Blastee command line argument number error\n");
    exit(1);
  }
  printf("port number is: %s + echo is: %s\n", argv[2], argv[4]);

  int blastee_socket;
  char buffer[50*1024 + 4*9];
  ssize_t recsize;
  socklen_t fromlen;

  //set blastee sa
  struct sockaddr_in sa;

  /* create an UDP socket*/
  if(-1 == (blastee_socket = socket(AF_INET, SOCK_DGRAM, 0)))
  {
    error("Socket() failed\n");
  }

  /* set socket parameter */
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  //TODO what is the INADDR_ANY here
  //sa.sin_addr.s_addr = inet_addr("128.105.37.194");
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  //printf("binding to address\n: %s", inet_ntoa(sa.sin_addr));
  sa.sin_port = htons(atoi(argv[2]));
  fromlen = sizeof(sa);

  //TODO
  struct sockaddr_in blaster_sa; 
  //char send_buff[50*1024 + 4*9];
  //int port_num = 7000;

  blaster_sa.sin_family = AF_INET;
  //blaster_sa.sin_addr.s_addr = sa.sin_addr.s_addr;
  //blaster_sa.sin_addr.s_addr = inet_addr(inet_ntoa(sa.sin_addr));
  //printf("TEST: %s", inet_ntoa(sa.sin_addr));
  blaster_sa.sin_port = htons(atoi(argv[2]));


  //printf("From host %s, port %hd, ",inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));

  /* bind socket to address */
  if(-1 == bind(blastee_socket, (struct sockaddr *) &sa, fromlen))
  {
    error("bind() failed\n");
  }

  struct timeval tv;

  tv.tv_sec = 5;  /* 5 Secs Timeout */
  tv.tv_usec = 0;  // Not init'ing this can cause strange errors

  if (-1 == setsockopt(blastee_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval)))
  {
    error("ERROR: setsockopt error");
  }

  /* keep receiving */
  //TODO implement END mechanism
  for (;;)
  {
    recsize = recvfrom(blastee_socket, (void*)buffer, sizeof buffer, 0, (struct sockaddr*)&sa, &fromlen);

    printf("receive success! ");
    printf("recsize = %d\n", recsize);
    /*
    // assume s is a connected socket
    socklen_t len;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    len = sizeof addr;
    if(-1 == getpeername(blastee_socket, (struct sockaddr*)&addr, &len))
    {
    error("ERROR: getpeername() failed!");
    }

    if (addr.ss_family == AF_INET) 
    {
    struct sockaddr_in *blastee_socket = (struct sockaddr_in *)&addr;
    port = ntohs(blastee_socket->sin_port);
    inet_ntop(AF_INET, &blastee_socket->sin_addr, ipstr, sizeof ipstr);
    } 
    else 
    { // AF_INET6
    struct sockaddr_in6 *blastee_socket = (struct sockaddr_in6 *)&addr;
    port = ntohs(blastee_socket->sin6_port);
    inet_ntop(AF_INET6, &blastee_socket->sin6_addr, ipstr, sizeof ipstr);
    }

    printf("Peer IP address: %s\n", ipstr);
    */

    if(recsize < 0)
    {
      error("error: recsive < 0, this can happen after 5 sec timeout\n");
    }

    char data;
    memcpy(&data, buffer, 1);    

    //char seq[4];
    //memcpy(seq, buffer+1, 4);
    long sequence;
    memcpy(&sequence, buffer+1, 4);
    sequence = ntohl(sequence);
    //sequence = atoi(*seq);

    //char len[4];
    //memcpy(len, buffer+5, 4);
    uint32_t length;
    memcpy(&length, buffer+5, 4);
    length = ntohl(length);
    //length = atoi(*len);

    char payload[50*1024];
    memcpy(payload, buffer+9, 50*1024);

    printf("received packet: ");
    printf("data= %c, ", data);
    printf("sequence= %lu, ", sequence);
    printf("length: %d, ", length);

    printf("payload: ");
    int i = 0;
    for(i = 0; i < 32; i++)
    {
      printf("%c", payload[i]);
    }
    printf("\n");

    if(data == 'C')
    {
/*
      struct timespec time, time2;
      time.tv_sec = 0;
      time.tv_nsec = 500L;


      if(nanosleep(&time, &time2) < 0 )
      {
	error("nanosleep() failed");
	time.tv_nsec = 0;
      }
      else
      {
	printf("Nano sleep successfull \n");
      }
*/

      //printf("this packet should be echoed\n");
      int bytes_sent = 0;

      blaster_sa.sin_addr.s_addr = inet_addr(inet_ntoa(sa.sin_addr));
      printf("TEST: %s", inet_ntoa(sa.sin_addr));
      //prtinf("TESTPORT: %hd", ntohs(sa.sin_port));

      bytes_sent = sendto(blastee_socket, buffer, sizeof buffer, 0, (struct sockaddr*)&blaster_sa, sizeof blaster_sa);



      printf("sent to host %s, port %hd, ",inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
      printf("bytes_sent is %d\n", bytes_sent);

    }
    else if (data == 'E')
    {
      break;
    }
  }//end of for loop of receving packet


  if (-1 == close(blastee_socket))
  {
    error("Oops, close() failed");
  }
  else
  {
    printf("Your socket has been successfully closed, thanks for using Xuyi-Yunhe socket!\n");
  }

  //TODO: return 0 for now, modify later
  return 0;
}
