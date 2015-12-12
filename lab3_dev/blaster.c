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
    char 	type;			//D = DATA, E = END, C = ECHO
    uint32_t 	sequence;	//increase in the number of payload octets
    uint32_t	length;			//number of octets in the payload
    char	payload[50*1024];	//vaiable size
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
  int blaster_socket;
  blaster_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if(-1 == blaster_socket)
  {
    error("socekt create failed");
  }
  //read the detailed information about the struct in previous comment
  //struct sockaddr_in sa;
  struct sockaddr_in blastee_sa;
  //data byte count
  socklen_t fromlen;
  //ssize_t recvsize;
  //TODO: How should I determine the size of the buffer
  //potbug: How to determine the size of the buffer
  int buf_size = 50*1024 + 4*9; //as length < 50KB
  char buffer[buf_size];
  //int msg_size = atoi(argv[12]);
  int bytes_sent;
  
  //construct first packet
  struct packet pkt0;
  pkt0.type = 'D';
  pkt0.sequence = atoi(argv[12]);
  printf("test pkt sequence: %d\n", pkt0.sequence);
  memset(pkt0.payload, 0, sizeof pkt0.payload);
  printf("this is the size of the payload: %d\n", sizeof pkt0.payload);
  strcpy(pkt0.payload, "This this is packet0");
  printf("test pkt payload: %s\n", pkt0.payload); 
  pkt0.length = strlen(pkt0.payload);
  printf("test pkt length: %d\n", pkt0.length);
  //convert packet to a string
  //char strToGo[50 * 1024 + 9 * 4];
  //memset(&strToGo, 0, sizeof strToGo);

  //pkt0.sequence = itoa(atoi(argv[12]));
  //pkt0.length = itoa(1);
  //itoa(atoi(argv[12]), &pkt0.sequence, 10);
  //itoa(1, &pkt0.length, 10);
  //pkt0.payload = 't';

  //buffer = pkt0;
  //strcpy(buffer, pkt0);
  //strcpy(buffer, "Hello world! Transmission success!");
  
  //memcpy(buffer, &pkt0, 9*4 + 50*1024);

  memcpy(buffer, &pkt0.type, 1);
  uint32_t seq;
  seq = htonl(pkt0.sequence);
  memcpy(buffer+1, &seq, 4);
  uint32_t len;
  len = htonl(pkt0.length);
  memcpy(buffer+5, &len, 4);
  memcpy(buffer+9, &pkt0.payload, sizeof pkt0.payload);

  int i = 0;
  for(i = 1; i < 5; i++)
  {
    //printf("buff[%d] is: %c\n", i, buffer[i]);
  }
 
  //int i = 0;
  for(i = 0; i < 100; i++)
  {
    printf("buffer[%d] is: %c\n", i, buffer[i]);
  }
  //char sequenceTest[4];
  //char lengthTest[4];
  //char payloadTest[50*1024];
  printf("sequence is %d\n", seq);
  printf("length is %d\n", len);
  printf("payload is %s\n", pkt0.payload);

  memset(&blastee_sa, 0, sizeof blastee_sa);

  blastee_sa.sin_family = AF_INET;
  blastee_sa.sin_addr.s_addr = inet_addr(argv[2]);
  blastee_sa.sin_port = htons(atoi(argv[4])); 
  //prinf("port number is : ");
  fromlen = sizeof(blastee_sa);

  bytes_sent = sendto(blaster_socket, buffer, sizeof buffer, 0, (struct sockaddr*)&blastee_sa, sizeof blastee_sa);
  printf("bytes_sent is %d\n", bytes_sent);
  printf("sent to host %s, port %hd\n",inet_ntoa(blastee_sa.sin_addr), ntohs(blastee_sa.sin_port));

 
  if(bytes_sent < 0)
  {
    error("bytes_sent < 0\n");
  }

  close(blaster_socket);
  return 0;
}
