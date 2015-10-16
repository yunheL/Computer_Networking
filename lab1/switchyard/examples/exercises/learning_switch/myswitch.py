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
import threading
import time

forwardList = {}
time_thread = threading.Timer(None, None)
exit = False

def switchy_main(net):
    my_interfaces = net.interfaces() 
    # interface() returns list of interface object.
    # mymacs holds all the MAC addrs stored in current network device
    # strcture for interface object: 
    # - intf.ipaddr -> ip address assoicate with the interface (IPv4Address object)
    # - intf.ethaddr -> mac address
    # - intf.name -> port/interface (eth0, eth1, etc)
    # - intf.netmask - network mask associated with the IPv4 address
    #mymacs = [intf.ethaddr.toStr() for intf in my_interfaces]
    
    # ?? intf.ethaddr is a ethaddr instance [Ethernet (ixx,xx,xx,x,x)]
    ''' 
    for intf in my_interfaces:
        print(dir(intf.ethaddr), exit())
    '''

    #print ("mymacs: ", mymacs)
    # TODO: construct a dict with <ethaddr : port> pair
    global forwardList, time_thread, exit
    forwardList = dict([(intf.ethaddr.toStr(), [intf.name, currnt_time_in_sec()+30]) for intf in my_interfaces])
    #print ("forwardList", forwardList)
    
    # call timeout functiion
    timeout()

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
        if srcAddr not in forwardList:
            #print ("src addr. not in forward table, add", srcAddr , "to table")
            print ("curren Time: ", currnt_time_in_sec())
            forwardList[srcAddr] = [dev, currnt_time_in_sec()+30]
            # update mymacs list
            print ("updated list: ", forwardList)
            #mymacs.append(srcAddr)
        #print ("src: ", packet[0].src)
        #print ("dst: ", packet[0].dst)
        #print ("forwardList: ", forwardList)
        if packet[0].dst.toStr() in forwardList:
            print ("packet with dst: ", packet[0].dst, " recognized by forwrad table")
            # packet[0] - header for the packet
            # other data in the header: dst, src, ethertype
            log_debug ("Packet intended for me")
            # TODO: look up port for dst addr. and forward the packet to that port.
            #print ("send packet to ", forwardList[packet[0].dst.toStr()][0], "interface")
            net.send_packet(forwardList[packet[0].dst.toStr()][0], packet)
        else:
            #print ("packet with dst: ", packet[0].dst, " NOT recognized by forwrad table. broadcast the packet")
            for intf in my_interfaces:
                if dev != intf.name:
                    log_debug ("Flooding packet {} to {}".format(packet, intf.name))
                    net.send_packet(intf.name, packet)
    # exit = True
    net.shutdown()


def timeout():
    global time_thread, forwardList, exit
    print (forwardList)
    # clean stale entries
    update_List = {}
    for key, value in forwardList.items():
        print ("entry: ", key, "value: ", value)
        if (value[1] > currnt_time_in_sec()):
            update_List[key] = value
    forwardList = update_List
    # timer thread for timeout
    if (not exit):
        time_thread = threading.Timer(2.0, timeout)
        time_thread.start()
        #print ("start new thread: ", threading.current_thread())
    else: 
        #print ("cancel current thread: ", threading.current_thread())
        time_thread.cancel()
    #print ("timeout: ", forwardList)

def currnt_time_in_sec():
    current_milli_time = lambda: int(round(time.time() * 1000))
    return int(current_milli_time()/1000)
