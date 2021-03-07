#include <stdio.h> // printf()

#include "args.h"
#include "nap_pac.h"


int main(int argc, char** argv) {

    struct args args;

    if (parse_args(argc, argv, &args) != 0) {
        return -1; // TODO document error codes
    }

    printf("Args: {\n"
            "\tfile: %s\n"
            "\tiface: %s\n"
            "\tpromisc: %s\n"
        "}\n",
        args.file,
        args.interface,
        args.promiscuous? "true":"false"
    );

    pcap_t* handle = init_pcap(&args);
    if (!handle) {
        return -2; // TODO document error codes
    }

    stop(handle);

    return 0;
}
