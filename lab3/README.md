### Briefer description :smiley:

*Blaster*: Send variable size UDP packets to a specified host  
*Blastee*: receive packets, log them and printing receip info

**Packet:**  
8-bit packet type: D, E, C  
32-bit sequence  
32-bit length  
variable length payload  

**Test:**  
any number of DATA + 1 END

**Invoke Blaster:**  
```
blaster -s <hostname> -p <port> -r <rate> -n <num> -q <seq_no> -l <length> -c <echo>
```

**Invoke Blastee:**  
```
blastee -p <port> -c <echo>
```

**Blastee print:**  
```
if(received is DATA)
	print packet info;
else if(received is END)
	print summary info;
else if(5 seconds time out)
	treat as END LOST;
	print summary info;
```

-> Excutable must be named "blaster" and "blastee"  
-> Need to provide Makefile:see_no_evil:  

**Advice:**  
-> Start Small:baby_chick:  
-> listening port test "netstat -u"  
-> Man page: getopt(3), ip(7), udp(7), sendto(2), recvfrom(2), bind(2), and gethostbyname(3) (e.g., "$ man 7 ip".)  
-> remember to check return value of each system call function  
-> "echo" service is port 7  

**Helpful Knowledge on Socket:**  
Endpoint of an inter-process communication  
Socket address is the combination of an IP address and a port number  
A socket is characterized by: 1. Local socket address (Local IP + Port#) 2. Protocol (e.g. UDP)  
A socket is referred to by a unique integer value called a socket descriptor
The netstate command line tool can be used to list all currently established sockets and related information  
![Very good TCP Socket flow diagram](https://upload.wikimedia.org/wikipedia/commons/a/a1/InternetSocketBasicDiagram_zhtw.png)


###Question about assignment description:stuck_out_tongue_closed_eyes:
1. So the size of <length> i.e. length of variable payload should stay the same for each test? (thus we can control the transmission rate by using the variable <rate> i.e number of packets per second)

2. The print out for blaster:
seq# payload(4 byte) <-on the same line
echo seq# payload(4 byte) <- on the same line
Is this correct? Is seq# and payload(4 bit) on the same line?

3. The ehco is only one way ("blastee -> blaster") is that correct? So even if the blatee blast back packet with echo field has the value "1", the blaster will not resend the packet.

###Questions during writing the program
1. How we do the server hear from client first? i.e. How to we realize that blaster will hear from blastee first? Since blaster is not a "famous port", how does blastee knows where to send?
