#include <stdlib.h> // exit()
#include <pcap/pcap.h>

#include "nap_pac.h"
#include "args.h" // struct args
#include "print_dlt.h" // print_dlt_name()


pcap_t* init_pcap(const struct args* args) {
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* handle;

    if (args->file) {
         handle =  pcap_open_offline(args->file, errbuf);
         if (handle == NULL) fprintf(stderr, "%s\n", errbuf);
         return handle;
    }

    handle = pcap_create(args->interface, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "%s\n", errbuf);
        return handle;
    }


    // Capture very big packets, but only on live captures
    pcap_set_snaplen(handle, 65535);

    pcap_set_promisc(handle, args->promiscuous);

    // Deliver packets as soon as they arrive, no buffering
    // This means higher CPU usage, but :blob_shrug:
    pcap_set_immediate_mode(handle, 1);


    const int status = pcap_activate(handle);
    if (status < 0) {
        // Errors, report and exit
        switch (status) {
            case PCAP_ERROR_PERM_DENIED:
                fprintf(stderr, "Error: permission denied for interface\n");
                fprintf(stderr, "You probably need to be root.\n");
                exit(-2); // TODO standardise errors
            case PCAP_ERROR_PROMISC_PERM_DENIED:
                fprintf(stderr, "Error: permission denied for promiscuity\n");
                exit(-3); // TODO standardise errors
            case PCAP_ERROR_NO_SUCH_DEVICE:
                fprintf(stderr, "Error: %s: no such device\n\n", args->interface);

                pcap_if_t* alldevs;
                pcap_findalldevs(&alldevs, errbuf);

                if (alldevs != NULL) {
                    fprintf(stderr,
                            "Please specify one of the following interfaces\n"
                           );
                    while (alldevs != NULL) {
                        fprintf(stderr, "%s\n", alldevs->name);
                        alldevs = alldevs->next;
                    }
                }
                fprintf(stderr, "\n");
                pcap_freealldevs(alldevs);
                exit(-4); // TODO standardise errors
            case PCAP_ERROR_IFACE_NOT_UP:
                fprintf(stderr, "Error: interface %s not up\n", args->interface);
                exit(-5); // TODO standardise errors
            default:
                exit(-1);
        }
    } else if (status > 0){
        // Warnings
    }

    int datalink_type = pcap_datalink(handle);
    printf("DLT: %d\n", datalink_type);
    return handle;
}


struct ether_addr* cap_pkt(pcap_t* handle) {
    int datalink_type = pcap_datalink(handle);

    struct pcap_pkthdr h;
    const unsigned char* pktdata = pcap_next(handle, &h);

    printf("Packet length: %d\n", h.caplen);

    print_dlt_name(datalink_type);

    return NULL;
}


void stop(pcap_t* handle){
    pcap_close(handle);
}
