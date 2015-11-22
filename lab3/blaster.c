/* Author: Xuyi Ruan, Yunhe Liu
 * Data: November 22, 2015
 * Acknowledgement: Some code in this file are
 * referenced from Linux Programmer's Manual (the Man Page)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

//getopt(): function parses the command-line arguments.

/*ip: Linux inplements the IPv4. An IP socket is created
 * by calling the socket functions as socket(AF_INET, socket_type, protocol)
 */
struct sockaddr_in 
{
  sa_family_t    sin_family; /* address family: AF_INET */
  in_port_t      sin_port;   /* port in network byte order */
  struct in_addr sin_addr;   /* internet address */
};

/* Internet address. */
struct in_addr {
  uint32_t       s_addr;     /* address in network byte order */
};

int main()
{
  //instantiate the udp socket
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

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
