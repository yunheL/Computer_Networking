#include <arpa/inet.h>
#include <stdio.h>

int main (int argc, char **argv) {
  uint32_t a;
  uint32_t a_n;
  uint32_t a_h;
  a = 10;
  a_n = htonl(a);
  
  printf("10: %d\n", a);
  printf("htonl(10): %d\n", a_n);
  a_h = ntohl(a_n);
  printf("ntohl(a_n): %d\n", a_h);
 
  a = 11;
  a_n = htonl(a);
  a_h = ntohl(a_n);

  printf("11: %d\n", a);
  printf("htonl(11): %d\n", a_n);
  printf("ntohl(a_n): %d\n", a_h);

}
