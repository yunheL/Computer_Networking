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
  printf("port number is: %s + echo is: %s\n", argv[2], argv[4]);

  int blastee_socket;
  struct sockaddr_in sa;
  char buffer[50*1024 + 4*9];
  //char send_buff[50*1024 + 4*9];
  ssize_t recsize;
  socklen_t fromlen;

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
  
  printf("binding to address: %s", inet_ntoa(sa.sin_addr));
  sa.sin_port = htons(atoi(argv[2]));
  fromlen = sizeof(sa);

  /* bind socket to address */
  if(-1 == bind(blastee_socket, (struct sockaddr *) &sa, fromlen))
  {
     error("bind() failed\n");
  }
  
  /* keep receiving */
  //TODO implement END mechanism
  for (;;)
  {
    //fflush(stdout);
    recsize = recvfrom(blastee_socket, (void*)buffer, sizeof buffer, 0, (struct sockaddr*)&sa, &fromlen);
 
    printf("receive success! ");
    printf("recsize = %d\n", recsize);
    
    if(recsize < 0)
    {
      error("error: recsive < 0");
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
      printf("this packet should be echoed\n"); 
    }
    else if (data == 'E')
    {
      break;
    }
  }//end of for loop of receving packet
/*
    //int i = 0;
    for(i = 0; i < 100; i++)
    {
      printf("buffer[%d] is: %c\n", i, buffer[i]);
    }
  }
*/
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
