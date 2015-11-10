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
        self.interface_ip = [str(intf.ipaddr) for intf in net.interfaces()]
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
            
            dst_addr = pkt[1].dst
            if str(dst_addr) in self.interface_ip:
                # drop packet
                # dst of packet (pkt[1].dst) same as router interface addr.
                #TODO: 
                continue

            match_list = []
            for entry in self.forward_table:
                subnet_num = IPv4Address(entry[0])
                mask = IPv4Address(entry[1])
                match = (int(dst_addr) & int(mask)) == int(subnet_num)
                if match:
                    match_list.append([int(subnet_num), entry[3]])
            
            longest_match = 0
            longest_index = 0
            if len(match_list) > 0: 
                # has match/matches
                if len(match_list) == 1:
                    # only one match
                    # TODO: 
                    continue
                for index, entry in enumerate(match_list):
                    if entry[0] > longest_match:
                        longest_index = index
                        longest_match = entry[0]
                # TODO: current reutrn interface
                match_list[longest_index][1]

            else: 
                # no match with forward table
                # TODO: 
                continue

            

def switchy_main(net):
    '''
    Main entry point for router.  Just create Router
    object and get it going.
    '''
    r = Router(net)
    r.router_main()
    net.shutdown()
