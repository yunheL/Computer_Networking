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

def switchy_main(net):
    my_interfaces = net.interfaces() 
    # TODO: construct a dict with <ethaddr : port> pair
    global forwardList, time_thread, exit
    forwardList = dict([(intf.ethaddr.toStr(), [intf.name, currnt_time_in_sec()+30]) for intf in my_interfaces]) 
    # call timeout functiion
    timeout()
    print ("main thread: ", threading.get_ident())
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
        # TODO: if the source MAC addr. not seen before, add it to the forward table.
        print ("main thread: ", threading.get_ident())
        srcAddr = packet[0].src.toStr()
        if srcAddr not in forwardList:
            forwardList[srcAddr] = [dev, currnt_time_in_sec()+30]
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
    global time_thread, forwardList
    # clean stale entries
    update_List = {}
    for key, value in forwardList.items():
        #print ("entry: ", key, "value: ", value)
        if (value[1] > currnt_time_in_sec()):
            update_List[key] = value
    forwardList = update_List
    # update timer thread for timeout
    if (threading.main_thread().is_alive()):
        # if main fucntion still alive
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
