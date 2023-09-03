from scapy.all import *



target_ip = "151.101.154.152"  
target_port = 443


def process_packet(packet):
    global pkt,count,flag
    if  packet.haslayer(IP) and packet.haslayer(TCP):
        src_ip = packet[IP].src
        dst_ip = packet[IP].dst
        src_port = packet[TCP].sport
        dst_port = packet[TCP].dport
        
        if (packet[TCP].flags & 0x01 and src_ip == target_ip ):
            pkt.append(packet)

            print()
            print(" Detected Fin,Ack from Server:")
            print(packet.summary())

            flag==1
            count+=1

        elif  (packet[TCP].flags & 0x01 and dst_ip == target_ip ):

            pkt.append(packet)
            
            print()
            print(" Detected Fin,Ack from Client:")
            print(packet.summary())

            flag==1
            count+=1

            
            
        elif flag==1:

            if (packet[TCP].flags == 0x10 and dst_ip == target_ip ):
                pkt.append(packet)
                print()
                print(" Ack from Client ")
                print(packet.summary())
                count=21
            else:
                pkt.append(packet)
                print()
                print(" Subsequent Pkt Detected")
                print(packet.summary())
                count+=1
                


def stopfilter(x):
    if count>20:
        wrpcap('HandshakeClose.pcap', pkt)
        return True
    else:
        return False

pkt=[]
count=0
flag=0


sniff(filter="tcp", prn=process_packet,stop_filter=stopfilter)


