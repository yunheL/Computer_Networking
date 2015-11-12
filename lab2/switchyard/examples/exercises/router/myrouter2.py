#!/usr/bin/env python3

'''
Basic IPv4 router (static routing) in Python.
'''

import sys
import os
import time
import ipaddress
from switchyard.lib.packet import *
from switchyard.lib.address import *
from switchyard.lib.common import *


class Router(object):
    def __init__(self, net):
        self.net = net
        self.interface_ip_mac = dict((str(intf.ipaddr), str(intf.ethaddr)) for intf in net.interfaces())
        self.ip_queue = {}
        # {ip addr : MAC addr} 
        self.arp_table = {}
        # other initialization stuff here
        #print (int(IPv4Address('192.168.1.0')))
        
        #print ("interface_ip", self.interface_ip)
        # import forward_table from interfaces
        
        # ip = ipaddress.ip_address(3221225986) returns 192.0.2.2

        self.forward_table = [[str(ipaddress.ip_address(int(intf.ipaddr) & int(intf.netmask))), str(intf.netmask), str(intf.ipaddr), intf.name] for intf in net.interfaces()]

        # import foward_table from files. 
        for intf in open('forwarding_table.txt').read().split('\n'):
            new_entry = [sub_entry for sub_entry in intf.split(' ')]
            #print ("new_entry: ", new_entry)
            self.forward_table.append(new_entry)
        #print (self.forward_table)
        #print (exit())
        print ("self.forward_table: ", self.forward_table)

    def router_main(self):    
        '''
        Main method for router; we stay in a loop in this method, receiving
        packets until the end of time.
        '''
        while True:
            gotpkt = True
            try:
                dev,pkt = self.net.recv_packet(timeout=1.0)
            except NoPackets:
                log_debug("No packets available in recv_packet")
                gotpkt = False
            except Shutdown:
                log_debug("Got shutdown signal")
                break

            if gotpkt:
                log_debug("Got a packet: {}".format(str(pkt)))
            
                     
            ip_pkt = pkt.get_header(IPv4)
            arp = pkt.get_header(Arp)
            

            # push pkt into queue if is IPv4 packet
            if ip_pkt is not None:
                # IPv4 pkt received
                print ("IPv4 received")
                # decrement TTL
                pkt[1].ttl = pkt[1].ttl-1
                
                # drop packet if dst same as interface addr.
                dst_addr = pkt[1].dst
                if str(dst_addr) in self.interface_ip_mac:
                    # drop packet
                    #TODO:
                    continue
                
                # TODO: if dst ip in Arp table
                # rtn packet with the ether_addr
                if str(dst_addr) in self.arp_table:
                    pkt[0].src = 
                    pkt[0].dst = self.arp_table[str(dst_addr)]
                    # need to get interface and src_mac,
 
                    self.net.send_packet(forward_entry[3], arppacket)

                # push ip pkt into queue
                self.ip_queue[str(dst_addr)] = [pkt, 0, time.time()]
                
                # check if has match in forward table.
                match_list = []
                for entry in self.forward_table:
                    subnet_num = IPv4Address(entry[0])
                    mask = IPv4Address(entry[1])
                    dst_addr = pkt[1].dst
                    match = (int(dst_addr) & int(mask)) == int(subnet_num)
                    if match:
                        match_list.append([int(subnet_num), entry])
                longest_match = 0
                longest_index = 0
                if len(match_list) > 0: 
                    # has match/matches
                    if len(match_list) == 1:
                        # only one match
                        # TODO: 
                        forward_entry = match_list[0][1]
                        dst_addr = pkt[1].dst
                        print ("from only 1") 
                        Router.arp_request(self, str(dst_addr), forward_entry)
                    else:
                        for index, entry in enumerate(match_list):
                            if entry[0] > longest_match:
                                longest_index = index
                                longest_match = entry[0]
                        # TODO: current reutrn interface
                        forward_entry = match_list[longest_index][1]
                        dst_addr = pkt[1].dst
                        print ("From more than 1")
                        Router.arp_request(self, str(dst_addr), forward_entry)
                         
                else: 
                    # no match with forward table
                    # TODO: 
                    continue
                 

            
            # handle ARP receive
            # 1. add to ARP table (cache)
            # 2. delete from queue
            if arp is not None: 
                # arp received
                print ("arp received")
                print ("ip_queue: ", self.ip_queue)
                send_ip = pkt[1].senderprotoaddr
                send_mac = pkt[1].senderhwaddr
                recv_mac = pkt[1].targethwaddr
                
                # cache entry to ARP table
                self.arp_table[str(send_ip)] = str(send_mac)
                print (self.arp_table)
                
                # update Ethernet header
                packet = self.ip_queue[str(send_ip)][0]
                packet[0].src = recv_mac
                packet[0].dst = send_mac

                # delete IP_pkt from queue
                del self.ip_queue[str(send_ip)]

                # send IP_pkt to dest host in layer2
                self.net.send_packet(dev, packet)
           
            # TODO: Constantly check packets inside queue.
 
    def arp_request(self, targetip, forward_entry):
        ether = Ethernet()
        # ?? what should ethernet source mac address be?
        ether.src = self.interface_ip_mac[forward_entry[2]]
        ether.dst = 'ff:ff:ff:ff:ff:ff'
        ether.ethertype = EtherType.ARP
        arp = Arp()
        arp.operation = ArpOperation.Request
        arp.senderhwaddr = self.interface_ip_mac[forward_entry[2]]
        arp.senderprotoaddr = forward_entry[2]
        arp.targethwaddr = 'ff:ff:ff:ff:ff:ff'
        arp.targetprotoaddr = targetip
        arppacket = ether + arp
        self.net.send_packet(forward_entry[3], arppacket)

def switchy_main(net):
    '''
    Main entry point for router.  Just create Router
    object and get it going.
    '''
    r = Router(net)
    r.router_main()
    net.shutdown()
