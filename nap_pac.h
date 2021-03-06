#ifndef NAP_PAC_H
#define NAP_PAC_H

#include <pcap/pcap.h> // pcap_t
#include <netinet/ether.h> // struct ethr_addr

#include "args.h" // struct args

pcap_t* init_pcap(const struct args* args);
struct ether_addr* cap_pkt(pcap_t* handle);
void stop(pcap_t* handle);

#endif
