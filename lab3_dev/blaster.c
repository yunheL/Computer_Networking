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
    long 	sequence;	//increase in the number of payload octets
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
  //struct sockaddr_in sa;
  struct sockaddr_in blastee_sa;
  //data byte count
  socklen_t fromlen;
  //ssize_t recvsize;
  int buf_size = 50*1024 + 4*9; //as length < 50KB
  char buffer[buf_size];
  //int msg_size = atoi(argv[12]);
  int bytes_sent;
  int packet_number = atoi(argv[8]);

  //construct packet
  struct packet pkt0;
 
  char *ptr;
  long unsign_seq;
  long prev_byte_sent;

  unsign_seq = strtoul(argv[12], &ptr, 10);
  printf("base seq is: %lu\n", unsign_seq);
  pkt0.sequence = unsign_seq;
  prev_byte_sent = 0;  

  int i = 0;
  for(i = 0; i<packet_number + 1;i++)
  { 
    //if this is already packet_num packet sent, this 
    //should be the END packet
    if(i == packet_number)
    {
      pkt0.type = 'E';
    }
    //otherwise it should be either D or C based on
    //the passed in echo value 
    else if(i < packet_number)
    {
      //if echo == 1
      if(atoi(argv[14]) == 1)
      {
	pkt0.type = 'C';
      }
      //if echo == 0
      else
      {
	pkt0.type = 'D';
      }	 
    }
    //if i gets larger than packet_number then we
    //have a problem 
    else
    {
      error("error in determining pakcet type");
    }

    pkt0.sequence = pkt0.sequence + prev_byte_sent;

    memset(pkt0.payload, 0, sizeof pkt0.payload);
    //strcpy(pkt0.payload, "This this is packet ");
    //TODO double check this number
    //memcpy(pkt0.payload+21, &i, 4);
    sprintf(pkt0.payload, "This is pakcet%d", i);
    pkt0.length = strlen(pkt0.payload);
    prev_byte_sent = pkt0.length;

   //copy pkt0 into buffer
    memcpy(buffer, &pkt0.type, 1);
    long seq;
    seq = htonl(pkt0.sequence);
    memcpy(buffer+1, &seq, 4);
    uint32_t len;
    len = htonl(pkt0.length);
    memcpy(buffer+5, &len, 4);
    memcpy(buffer+9, &pkt0.payload, sizeof pkt0.payload);

  /*
    //int i = 0;
    for(i = 0; i < 100; i++)
    {
      printf("buffer[%d] is: %c\n", i, buffer[i]);
    }
  */
    memset(&blastee_sa, 0, sizeof blastee_sa);

    blastee_sa.sin_family = AF_INET;
    blastee_sa.sin_addr.s_addr = inet_addr(argv[2]);
    blastee_sa.sin_port = htons(atoi(argv[4])); 
    //prinf("port number is : ");
    fromlen = sizeof(blastee_sa);

    bytes_sent = sendto(blaster_socket, buffer, sizeof buffer, 0, (struct sockaddr*)&blastee_sa, sizeof blastee_sa);


    printf("sent to host %s, port %hd, ",inet_ntoa(blastee_sa.sin_addr), ntohs(blastee_sa.sin_port));
    printf("bytes_sent is %d\n", bytes_sent);
    
    printf("sent packet: ");
    printf("data = %c, ", pkt0.type);
    /*
    //this is the encrypted set for testing
    printf("sequence is %d, ", seq);
    printf("length is %d, ", len);
    printf("payload is %s\n", pkt0.payload);
    */
    printf("sequence= %lu, ", pkt0.sequence);
    printf("length= %d, ", pkt0.length);
    printf("payload= %s, ", pkt0.payload); 
    printf("\n");
 
    if(bytes_sent < 0)
    {
      error("bytes_sent < 0\n");
    }
  }//end of for for sending pakcets
  close(blaster_socket);
  return 0;
}
