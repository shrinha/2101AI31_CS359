from scapy.all import *


target_ip = "151.101.154.152"  # www.scribd.com
target_port = 443  # port number

# Define a function to capture and print the 3-way handshake
def capture_3_way_handshake(packet):
    global handshake_completed
    global handshake
    
    if not handshake_completed and packet.haslayer(IP) and packet.haslayer(TCP):
        src_ip = packet[IP].src
        dst_ip = packet[IP].dst
        src_port = packet[TCP].sport
        dst_port = packet[TCP].dport

        if (
            dst_ip == target_ip
            and dst_port == target_port
            and packet[TCP].flags & 0x02
        ):
            print("SYN Packet:")
            print(packet.summary())
            handshake.append(packet)
        elif (
            src_ip == target_ip
            and src_port == target_port
            and packet[TCP].flags & 0x12 == 0x12
        ):
            
            print("SYN-ACK Packet:")
            print(packet.summary())
            handshake.append(packet)
            
        elif (
            dst_ip == target_ip
            and dst_port == target_port
            and packet[TCP].flags & 0x10
        ):
            print("ACK Packet:")
            print(packet.summary())
            handshake.append(packet)
            
            print("3-way handshake completed!")
            handshake_completed = True
            wrpcap('3WayHandshake.pcap', handshake)

            


interface = "Intel(R) Wi-Fi 6E AX211 160MHz"  
filter_expression = f"host {target_ip} and port {target_port}"
print("Capturing ")

handshake_completed = False   
handshake=[]



sniff(iface=interface, filter=filter_expression, prn=capture_3_way_handshake)

