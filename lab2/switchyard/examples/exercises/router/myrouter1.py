#!/usr/bin/env python3

'''
Basic IPv4 router (static routing) in Python.
'''

import sys
import os
import time
from switchyard.lib.packet import *
from switchyard.lib.address import *
from switchyard.lib.common import *

class Router(object):
    def __init__(self, net):
        self.net = net
        # other initialization stuff here
        my_interfaces = net.interfaces()
        self.forward_table = dict([(str(intf.ipaddr), intf.ethaddr.toStr()) for intf in my_interfaces])
        print (my_interfaces)
        print (exit())



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
            
            arp = pkt.get_header(Arp)
            if arp is None:
                log_debug("not arp, dropped")
                continue
            dst_ip_addr = str(arp.targetprotoaddr)

            if dst_ip_addr in self.forward_table:
                # response with ARP response
                targethwaddr = self.forward_table[dst_ip_addr]
                # send ARP response pkt
                reply_pkt = create_ip_arp_reply(targethwaddr, arp.senderhwaddr, arp.targetprotoaddr, arp.senderprotoaddr)
                self.net.send_packet(dev, reply_pkt)

            else:
                # should not response ARP
                continue
                print ("ip addr not in list of interfaces")


def switchy_main(net):
    '''
    Main entry point for router.  Just create Router
    object and get it going.
    '''
    r = Router(net)
    r.router_main()
    net.shutdown()
