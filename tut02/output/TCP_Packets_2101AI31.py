from scapy.all import *

# Initialize a counter to track received packets
packet_count = 0

# Define a function to process each captured packet
def process_packet(packet):
    global packet_count,src_ip,dst_ip,pkt_list
    if TCP in packet:
        if packet_count==0 :
            src_ip= packet[IP].src
            dst_ip= packet[IP].dst

            print("TCP1")
            print(packet.summary())
            pkt_list.append(packet)

            packet_count += 1
        
        elif packet[IP].src == dst_ip and packet[IP].dst == src_ip:
            print("TCP2")
            print(packet.summary())
            pkt_list.append(packet)

            wrpcap('TCP2.pcap', pkt_list)
            
            packet_count += 1


def stopfilter(x):
    if packet_count>=2:
        return True
    else:
        return False

pkt_list=[]


interface = "Intel(R) Wi-Fi 6E AX211 160MHz"
sniff(iface= interface , prn=process_packet,stop_filter=stopfilter)


