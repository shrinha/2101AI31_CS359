#include<bits/stdc++.h>
#include<pcap.h>
#include<stdlib.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
using namespace std;

void packetHandler(u_char* user,const pcap_pkthdr* pkthhdr,const u_char* packet){
    
    pcap_dump(user,pkthhdr,packet);
    cout<<"Packet Captured"<<endl;
}

int f1,f2,f3;
uint32_t ip_address;
uint32_t src_port;

void packetHandler_tcp_3_way_hanshake(u_char* user,const pcap_pkthdr* pkthhdr,const u_char* packet){
    struct iphdr * ip = (struct iphdr *)(packet+14);
    struct tcphdr * tcp = (struct tcphdr *)(packet + 14 + ((ip->ihl)<<2) );
    if(ip->protocol == IPPROTO_TCP){
        if(tcp->syn && !tcp->ack && ip->daddr==inet_addr("164.100.160.96") && f1==0){
            pcap_dump(user,pkthhdr,packet);
            f1=1;
            src_port = ntohs(tcp->source);
        }
        if(f1==0){return;}
        else if(tcp->syn && tcp->ack && ip->saddr == inet_addr("164.100.160.96") && f2==0 && ntohs(tcp->dest)==src_port){
            pcap_dump(user,pkthhdr,packet);
            f2=1;
        }
        if(f2==0){return;}
        else if(!tcp->syn && tcp->ack && ip->daddr==inet_addr("164.100.160.96") && f3==0 && ntohs(tcp->source)==src_port){
            pcap_dump(user,pkthhdr,packet);
            f3=1;
        }
    }
}

void packetHandler_tcp_hanshake_close(u_char* user,const pcap_pkthdr* pkthhdr,const u_char* packet){
    struct iphdr * ip = (struct iphdr *)(packet+14);
    struct tcphdr * tcp = (struct tcphdr *)(packet + 14 + ((ip->ihl)<<2) );
    if(ip->protocol == IPPROTO_TCP){
        if(tcp->fin && tcp->ack && ip->daddr==inet_addr("164.100.160.96") && f1==0 && ntohs(tcp->source)==src_port){
            pcap_dump(user,pkthhdr,packet);
            f1=1;
        }
        if(f1==0){return;}
        else if(tcp->fin && tcp->ack && ip->saddr == inet_addr("164.100.160.96") && f2==0 && ntohs(tcp->dest)==src_port){
            pcap_dump(user,pkthhdr,packet);
            f2=1;
        }
        if(f2==0){return;}
        else if(!tcp->syn && !tcp->fin && tcp->ack && ip->daddr==inet_addr("164.100.160.96") && f3==0 && ntohs(tcp->source)==src_port){
            pcap_dump(user,pkthhdr,packet);
            f3=1;
        }
    }
}

void packetHandler_tcp_packets(u_char* user,const pcap_pkthdr* pkthhdr,const u_char* packet){
    struct iphdr * ip = (struct iphdr *)(packet+14);
    struct tcphdr * tcp = (struct tcphdr *)(packet + 14 + ((ip->ihl)<<2) );
    if(ip->protocol == IPPROTO_TCP){
        if(f1==0){
            pcap_dump(user,pkthhdr,packet);
            ip_address = ip->daddr;
            f1+=1;
        }
        else if(f1==1){
            if(ip->saddr==ip_address){
                pcap_dump(user,pkthhdr,packet);
                f1+=1;
            }
        }
        else{
            return;
        }
    }
}

void packetHandler_udp_packets(u_char* user,const pcap_pkthdr* pkthhdr,const u_char* packet){
    struct iphdr * ip = (struct iphdr *)(packet+14);
    struct tcphdr * tcp = (struct tcphdr *)(packet + 14 + ((ip->ihl)<<2) );
    if(ip->protocol == IPPROTO_UDP){
        if(f1==0){f1+=1;return;}
        if(f1==1){
            pcap_dump(user,pkthhdr,packet);
            ip_address = ip->daddr;
            f1+=1;
        }
        else if(f1==2){
            if(ip->saddr==ip_address){
                pcap_dump(user,pkthhdr,packet);
                f1+=1;
            }
        }
        else{
            return;
        }
    }
}

void tcp_3_way_handshake_capture(){
    f1=0,f2=0,f3=0;
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    char pcap_file[]="output.pcap";

    handle = pcap_open_offline(pcap_file,errbuf);

    if(handle==NULL){
        fprintf(stderr,"Error opening pcap file : %s\n",errbuf);
        exit(1);
    }
    pcap_dumper_t * dumper = pcap_dump_open(handle,"TCP_3_way_handshake_start_2101AI31.pcap");

    if(dumper == NULL ){
        fprintf(stderr,"Error opening pcap dump file\n");
        exit(1);
    }

    if(pcap_loop(handle,0,packetHandler_tcp_3_way_hanshake,(u_char*)dumper)<0){
        cout<<"completed"<<endl;
    }

    pcap_close(handle);
}

void tcp_closing_handshake_capture(){
    f1=0,f2=0,f3=0;
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    char pcap_file[]="output.pcap";

    handle = pcap_open_offline(pcap_file,errbuf);

    if(handle==NULL){
        fprintf(stderr,"Error opening pcap file : %s\n",errbuf);
        exit(1);
    }
    pcap_dumper_t * dumper = pcap_dump_open(handle,"TCP_handshake_close_2101AI31.pcap");

    if(dumper == NULL ){
        fprintf(stderr,"Error opening pcap dump file\n");
        exit(1);
    }

    if(pcap_loop(handle,0,packetHandler_tcp_hanshake_close,(u_char*)dumper)<0){
        cout<<"completed"<<endl;
    }

    pcap_close(handle);
}

void tcp_packets(){
    f1=0,f2=0,f3=0;
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    char pcap_file[]="output.pcap";

    handle = pcap_open_offline(pcap_file,errbuf);

    if(handle==NULL){
        fprintf(stderr,"Error opening pcap file : %s\n",errbuf);
        exit(1);
    }
    pcap_dumper_t * dumper = pcap_dump_open(handle,"TCP_Packets_2101AI31.pcap");

    if(dumper == NULL ){
        fprintf(stderr,"Error opening pcap dump file\n");
        exit(1);
    }

    if(pcap_loop(handle,0,packetHandler_tcp_packets,(u_char*)dumper)<0){
        cout<<"completed"<<endl;
    }

    pcap_close(handle);
}

void udp_packets(){
    f1=0,f2=0,f3=0;
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    char pcap_file[]="output.pcap";

    handle = pcap_open_offline(pcap_file,errbuf);

    if(handle==NULL){
        fprintf(stderr,"Error opening pcap file : %s\n",errbuf);
        exit(1);
    }
    pcap_dumper_t * dumper = pcap_dump_open(handle,"UDP_Packets_2101AI31.pcap");

    if(dumper == NULL ){
        fprintf(stderr,"Error opening pcap dump file\n");
        exit(1);
    }

    if(pcap_loop(handle,0,packetHandler_udp_packets,(u_char*)dumper)<0){
        cout<<"completed"<<endl;
    }

    pcap_close(handle);
}

int main(){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;
    handle = pcap_open_live("enp0s3",BUFSIZ,1,1000,errbuf);

    if(handle == nullptr){
        cerr<<"Error opening capture interface: "<<errbuf<<endl;
        return 1;
    }

    pcap_dumper_t * dumper = pcap_dump_open(handle,"output.pcap");

    if(dumper == NULL ){
        fprintf(stderr,"Error opening pcap dump file\n");
        exit(1);
    }
    
    if(pcap_loop(handle,1000,packetHandler,(u_char*)dumper)<0){
        fprintf(stderr,"Error in pcap_loop\n");
        exit(1);
    }
    pcap_dump_close(dumper);
    pcap_close(handle);


    tcp_3_way_handshake_capture();
    tcp_closing_handshake_capture();
    tcp_packets();
    udp_packets();


    return 0;

}
