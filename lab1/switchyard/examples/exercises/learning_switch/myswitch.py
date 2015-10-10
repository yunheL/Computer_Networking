#!/usr/bin/env python3

'''
Ethernet learning switch in Python: HW3.

Note that this file currently has the code to implement a "hub"
in it, not a learning switch.  (I.e., it's currently a switch
that doesn't learn.)
'''
from switchyard.lib.address import *
from switchyard.lib.packet import *
from switchyard.lib.common import *

def switchy_main(net):
    my_interfaces = net.interfaces() 
    # interface() returns list of interface object.
    # mymacs holds all the MAC addrs stored in current network device
    # strcture for interface object: 
    # - intf.ipaddr -> ip address assoicate with the interface (IPv4Address object)
    # - intf.ethaddr -> mac address
    # - intf.name -> port/interface (eth0, eth1, etc)
    # - intf.netmask - network mask associated with the IPv4 address
    mymacs = [intf.ethaddr.toStr() for intf in my_interfaces]
    
    # ?? intf.ethaddr is a ethaddr instance [Ethernet (ixx,xx,xx,x,x)]
    ''' 
    for intf in my_interfaces:
        print(dir(intf.ethaddr), exit())
    '''

    print ("mymacs: ", mymacs)
    # TODO: construct a dict with <ethaddr : port> pair
    forwardList = dict([(intf.ethaddr.toStr(), intf.name) for intf in my_interfaces])
    print ("forwardList", forwardList)
    while True:
        try:
            # dev -> device name
            # packet -> packet receive
            dev,packet = net.recv_packet()
        except NoPackets:
            continue
        except Shutdown:
            return

        log_debug ("In {} received packet {} on {}".format(net.name, packet, dev))
        # log_debug ("I am here!")
        # TODO: if the source MAC addr. not seen before, add it to the forward table.
        srcAddr = packet[0].src.toStr()
        if srcAddr not in mymacs:
            print ("src addr. not in forward table, add", srcAddr , "to table")
            forwardList[srcAddr] = dev
            # update mymacs list
            mymacs.append(srcAddr)
        print ("src: ", packet[0].src)
        print ("dst: ", packet[0].dst)
        print ("forwardList: ", forwardList)
        if packet[0].dst.toStr() in mymacs:
            print ("packet with dst: ", packet[0].dst, " recognized by forwrad table")
            # packet[0] - header for the packet
            # other data in the header: dst, src, ethertype
            log_debug ("Packet intended for me")
            # TODO: look up port for dst addr. and forward the packet to that port.
            print ("send packet to ", forwardList[packet[0].dst.toStr()], "interface")
            net.send_packet(forwardList[packet[0].dst.toStr()], packet)
        else:
            print ("packet with dst: ", packet[0].dst, " NOT recognized by forwrad table. broadcast the packet")
            for intf in my_interfaces:
                if dev != intf.name:
                    log_debug ("Flooding packet {} to {}".format(packet, intf.name))
                    net.send_packet(intf.name, packet)
    net.shutdown()
