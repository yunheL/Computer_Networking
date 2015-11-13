## cs640lab2 - build your own smart router with switchyard

### CS640 2015 Fall
### Team: [Yunhe Liu](https://github.com/yunheL), [Xuyi Ruan](https://github.com/ruanxuyi)

### [Lab2 Link: ] (http://pages.cs.wisc.edu/~sskausik08/640-assignments/a2.html)

### [part1 instruction](https://github.com/jsommers/switchyard/blob/master/examples/exercises/router/router1.rst)


## Description: 

This project designed to understand the basic packet forwaring technique to reqpresent how a router work. The current version of the router only manage packet type for ARP and IPv4.


## TODO (part 1):
Response to ARP requests
~~1. determine the packet type (ARP)~~
~~2. drop packets that are not ARP for part1 of the project.~~

## Questions:
what do we do if the ip addr not in list of interface?

## TODO (part 2):

~~1. import forward table from interface and files. ~~  
~~2. forward match packet to dest.~~  

~~3. You should just ignore these non-IPv4 and non-ARP packets (and your router should not crash when it receives them!)~~  
~~4. decrement TTL.~~  
~~5. construct Ethernet header with `dst_mac` (from ARP inquery) ~~
~~7. handle received ARP query, if pkt receive type == ARP, 1) complete ethernet header 2) foward packet 3) added entry to the ARP table <ip, ethnet_addr> (local)~~  
~~8. send ARP once every second util recive OR excess max 5~~  


## Questions: 
1. the idea of next hop VS interfaces
- `next hop ip` is the ip address = the `sender_ip` for ARP request.  
- `interface mac` is the = `sender_mac` for ARP request. 
thnk of next hop ip is the next router in the sub net
 
2. ip addr for interface = next hop addr?
- yes  

3. the interface address = destination address -> packet to router itself?
- yes, when destination address == interface address, packet dropped.  

~~4. how to do bitwise and and return a subnet address for interfaces?~~
- by using `ipaddress.ip_address(int)`  

??? 5. why `network prefix` not match with `next hoop addr` ?

```
	172.16.0.0 255.255.0.0 192.168.1.2 router-eth0
	172.16.128.0 255.255.192.0 10.10.0.254 router-eth1
	172.16.64.0 255.255.192.0 10.10.1.254 router-eth1
	10.100.0.0 255.255.0.0 172.16.42.2 router-eth2

```


6. when a packet receive, do we need to deterine if it is IP pkts beofore we send ARP requests?
- need to determin if it is IPv4 or ARP for different operation

?? 7. how to find router IP? 
	IP address on a router through which the destination is reachable.

	

## Boundry conditions: 
1. router receive the ip address of router itself.
2. what do we do if the ip addr not in list of interface?:


## Important point: 
 for the arp_request.
`next_hop ip` - is the `sender ip`for next hop
'subnet_etherAddr' - is the sender ether_addr for next hop
next_hop help us to send the arp to the dest_host.
