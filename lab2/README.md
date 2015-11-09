## cs640lab2 - build your own smart router with switchyard

### CS640 2015 Fall
### Team: [Yunhe Liu](https://github.com/yunheL), [Xuyi Ruan](https://github.com/ruanxuyi)

### [Lab2 Link: ] (http://pages.cs.wisc.edu/~sskausik08/640-assignments/a2.html)

### [part1 instruction](https://github.com/jsommers/switchyard/blob/master/examples/exercises/router/router1.rst)



## TODO (part 1):
Response to ARP requests
~~1. determine the packet type (ARP)~~
~~2. drop packets that are not ARP for part1 of the project.~~

## Questions:
what do we do if the ip addr not in list of interface?

## TODO (part 2):

1. import forward table from interface and files. 
2. forward match packet to dest.


## Questions: 
1. the idea of next hop VS interfaces
2. ip addr for interface = next hop addr?
3. the interface address = destination address -> packet to router itself? what is router have many addresses? 
4. how to do bitwise and and return a subnet address for interfaces? 
5. why `network prefix` not match with `next hoop addr` ?

```
	172.16.0.0 255.255.0.0 192.168.1.2 router-eth0
	172.16.128.0 255.255.192.0 10.10.0.254 router-eth1
	172.16.64.0 255.255.192.0 10.10.1.254 router-eth1
	10.100.0.0 255.255.0.0 172.16.42.2 router-eth2

```

## Boundry conditions: 
1. router receive the ip address of router itself.
2. what do we do if the ip addr not in list of interface?:

