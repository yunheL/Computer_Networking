#!/usr/bin/env python

import sys
from switchyard.lib.address import *
from switchyard.lib.packet import *
from switchyard.lib.common import *
from switchyard.lib.testing import *

def mk_pkt(hwsrc, hwdst, ipsrc, ipdst, reply=False):
    ether = Ethernet()
    ether.src = EthAddr(hwsrc)
    ether.dst = EthAddr(hwdst)
    ether.ethertype = EtherType.IP

    ippkt = IPv4()
    ippkt.srcip = IPAddr(ipsrc)
    ippkt.dstip = IPAddr(ipdst)
    ippkt.protocol = IPProtocol.ICMP
    ippkt.ttl = 32

    icmppkt = ICMP()
    if reply:
        icmppkt.icmptype = ICMPType.EchoReply
    else:
        icmppkt.icmptype = ICMPType.EchoRequest

    return ether + ippkt + icmppkt

def switch_tests():
    s = Scenario("switch tests")
    s.add_interface('eth0', '10:00:00:00:00:01')
    s.add_interface('eth1', '10:00:00:00:00:02')
    s.add_interface('eth2', '10:00:00:00:00:03')


    # test case 0: a frame from A to interface's MAC - should drop packet
    # all ports except ingress
    testpkt = mk_pkt("00:00:00:00:00:aa", "10:00:00:00:00:01", "10.0.0.1", "10.0.0.12")
    s.expect(PacketInputEvent("eth0", testpkt, display=Ethernet), "An Ethernet frame fomr 00:00:00:00:00:aa to 10:00:00:00:00:01 should arrive on eth0")
 
    

    # test case 1: a frame from A to D (f_table not learned dest addr yet)
    # all ports except ingress
    testpkt = mk_pkt("00:00:00:00:00:aa", "00:00:00:00:00:dd", "10.0.0.1", "10.0.0.4")
    s.expect(PacketInputEvent("eth0", testpkt, display=Ethernet), "An Ethernet frame fomr 00:00:00:00:00:aa to 00:00:00:00:00:dd should arrive on eth0")
    s.expect(PacketOutputEvent("eth1", testpkt, "eth2", testpkt, display=Ethernet), "The Ethernet frame for 00:00:00:00:00:dd should be flouded out eth1 and eth2")

    # test case 1.1: a frame from A to D, A change interface number.
    testpkt = mk_pkt("00:00:00:00:00:aa", "00:00:00:00:00:dd", "10.0.0.1", "10.0.0.4")
    s.expect(PacketInputEvent("eth1", testpkt, display=Ethernet), "An Ethernet frame fomr 00:00:00:00:00:aa to 00:00:00:00:00:dd should arrive on eth1")
    s.expect(PacketOutputEvent("eth0", testpkt, "eth2", testpkt, display=Ethernet), "The Ethernet frame for 00:00:00:00:00:dd should be flouded out eth0 and eth2")
 
    
    # timeout for 20 sec
    s.expect(PacketInputTimeoutEvent(20.0), "Time out for 20 seconds")
    
    # test case 2: a frame from D to A (learned)
    testpkt = mk_pkt("00:00:00:00:00:dd", "00:00:00:00:00:aa", "10.0.0.4", "10.0.0.1")
    s.expect(PacketInputEvent("eth2", testpkt, display=Ethernet), "An Ethernet frame from 00:00:00:00:00:dd should arrive on eth2")
    s.expect(PacketOutputEvent("eth1", testpkt, display=Ethernet), "The Ethernet frame for 00:00:00:00:00:aa should be flouded on eth1")

    # test case 3: a frame from C to boardcast
    reqpkt = mk_pkt("00:00:00:00:00:cc", "ff:ff:ff:ff:ff:ff", "10.0.0.3", "255.255.255.255")
    s.expect(PacketInputEvent("eth1", reqpkt, display=Ethernet), "An Ethernet frame from 00:00:00:00:00:cc with a broadcast destination address should arrive on eth1")
    s.expect(PacketOutputEvent("eth0", reqpkt, "eth2", reqpkt, display=Ethernet), "The Ethernet frame with a broadcast destination address should be forward out ports eth0 and eth2")

    # timeout for 15 sec
    s.expect(PacketInputTimeoutEvent(15.0), "Time out for 15 seconds")
    
    # test case 4: a frame from C to A (learned, but forget)
    testpkt = mk_pkt("00:00:00:00:00:cc", "00:00:00:00:00:aa", "10.0.0.3", "10.0.0.1")
    s.expect(PacketInputEvent("eth1", testpkt, display=Ethernet), "An Ethernet frame from 00:00:00:00:00:dd should arrive on eth2")
    s.expect(PacketOutputEvent("eth0", testpkt, "eth2", testpkt, display=Ethernet), "The Ethernet frame for 00:00:00:00:00:aa should be flouded on eth0 and eth1")

    # test case 5: a frame from A to C (learned)
    testpkt = mk_pkt("00:00:00:00:00:aa", "00:00:00:00:00:cc", "10.0.0.1", "10.0.0.3")
    s.expect(PacketInputEvent("eth0", testpkt, display=Ethernet), "An Ethernet frame from 00:00:00:00:00:aa should arrive on eth0")
    s.expect(PacketOutputEvent("eth1", testpkt, display=Ethernet), "The Ethernet frame for 00:00:00:00:00:cc should be flouded on eth1")

    return s

scenario = switch_tests()
