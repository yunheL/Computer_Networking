#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>

#define EVER ;;
//function that generates error message
void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{

 //this struct construct the packet to be sent
  struct packet
  {
    char 	type;		//D = DATA, E = END, C = ECHO
    uint32_t 	sequence;	//increase in the number of payload octets
    uint32_t	length;		//number of octets in the payload
    char	*payload;	//vaiable size
  };

  //valid input command
  if (argc != 15)
  {
    error("Blaster command line argument number error\n");
    exit(1);
  }  

  printf("host: %s, port: %s\n", argv[2], argv[4]);
  //instantiate the udp socket
  //socket(AF_INET, socket_type, protocol),read more on Man page
  int blaster_socket = socket(AF_INET, SOCK_DGRAM, 0);
  //read the detailed information about the struct in previous comment
  struct sockaddr_in sa;
  struct sockaddr_in blastee_sa;
  //data byte count
  socklen_t fromlen;
  //ssize_t recvsize;
  //TODO: How should I determine the size of the buffer
  //potbug: How to determine the size of the buffer
  int buf_size = 50*1024; //as length < 50KB
  char buffer[buf_size];
  //int msg_size = atoi(argv[12]);
  int bytes_sent;

  
  strcpy(buffer, "Hello world! Transmission success!");
  //a.b when a is an object; a->b when a is a pointer to an object
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  //potbug: htonl, does we use long?
  //htonl() functoin converts the unsigned integer hostlong from host byte
  //potbug: INADDR_ANY
  sa.sin_addr.s_addr = inet_addr(argv[2]);
  sa.sin_port = htons(atoi(argv[4]));
  fromlen = sizeof(sa);

  if(-1 == connect(blaster_socket, (struct sockaddr *) &sa, fromlen))
  {
     error("bind() failed\n"); 
  }

  blastee_sa.sin_addr.s_addr = inet_addr(argv[2]);
  blastee_sa.sin_port = htons(atoi(argv[4])); 

  bytes_sent = sendto(blaster_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&sa, sizeof sa);
  
  if(bytes_sent < 0)
  {
    error("bytes_sent < 0\n");
  }

  close(blaster_socket);
  return 0;
}
