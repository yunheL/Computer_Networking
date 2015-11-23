/* Author: Xuyi Ruan, Yunhe Liu
 * Data: November 22, 2015
 * Acknowledgement: Some code in this file are
 * referenced from Linux Programmer's Manual (the Man Page)
 * and stackoverflow, and wikipedia
 */
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

/* steps of using sockets:
 * 1. Create the socket
 * 2. Identify the socket (name it)
 * 3. On the server, wait for a message
 * 4. On the client, send a message
 * 5. send a response back to the client (optional)
 * 6. close the socket
 */

//getopt(): function parses the command-line arguments.

//TODO: ask TA about code use from Wikipedia Berkeley Page
//TODO: Check return values of each function to eliminate error

/*ip: Linux inplements the IPv4. An IP socket is created
 * by calling the socket functions as socket(AF_INET, socket_type, protocol)
 */

//struct sockaddr_in 
//{
//  sa_family_t    sin_family; /* address family: AF_INET */
//  in_port_t      sin_port;   /* port in network byte order */
//  struct in_addr sin_addr;   /* internet address */
//};


/* Internet address. */
//struct in_addr {
//  uint32_t       s_addr;     /* address in network byte order */
//};

//function that generates error message
void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  //valid input command
  if (argc != 15)
  {
    error("Command line argument number error\n");
    exit(1);
  }  

  //instantiate the udp socket
  //socket(AF_INET, socket_type, protocol),read more on Man page
  int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  //read the detailed information about the struct in previous comment
  struct sockaddr_in sa;
  //data byte count
  socklen_t fromlen;

  //a.b when a is an object; a->b when a is a pointer to an object
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  //htonl() functoin converts the unsigned integer hostlong from host byte
  //order to network byte order
  //potbug: htonl, does we use long?
  //potbug: INADDR_ANY
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  //set port number as specified by command line, because port number is 
  //in the range[1024, 65536], so 16-bit short should be enough. Also
  //specified on textbook page.393
  sa.sin_port = htons(atoi(argv[5]));
  fromlen = sizeof(sa);

  //bind() assigns the address specified by addr to the socket referred to by
  //the file descriptor sockfd. addrlen spcifies the size, in bytes, of the address
  //structure pointed to by addr.
  //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);  
  if(-1 == bind(udp_socket, (struct sockaddr *) &sa, fromlen))
  {
     error("bind() failed\n"); 
  }

/*debug code
  printf("sizeof(sa) is %d\n", sizeof(sa));
  printf("fromlen is %d\n", fromlen);
*/


/* when a UDP socket is created, its local and remote addresses are unspecified.
 * Datagrams can be sent immediately using sendto() or sendmsg() 
 * with a valid destination address as an argument.
 * When connect() is called on the socket, the default destination
 * address is set and datagrams can now be sent using send() or write() without
 * specifying a destination address.
 * In order to receive packets, the socket can be bound to a local address first
 * by using bind()
 */

  return 0;
}
