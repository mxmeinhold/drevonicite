#include <stdio.h> // fprintf()
#include <unistd.h> // getopt()
#include <string.h> // memset()

#include "args.h"


int parse_args(int argc, char** argv, struct args* args) {
    // Reset all args to defaults
    memset(args, 0, sizeof(struct args));

    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch ((char) opt) {
            case 'f':
                args->file = optarg;
                break;
            case '?':
                goto failure;
            case ':':
                goto failure;
        }
    }

    if (!args->file) {
        if (optind == argc) {
            // No data specified
            // TODO list the available interfaces
            fprintf(stderr, "Interface or filepath expected.\n");
            goto failure;
        } else {
            // We have at least one acceptable interface
            args->interface = argv[optind];
            if (optind != argc - 1) {
                fprintf(stderr, "Multiple interfaces specified?\n");
                fprintf(stderr, "Proceeding using interface `%s`\n", args->interface);
            }
        }
    }

    return 0;

failure:
    fprintf(stderr, "Usage: %s interface_name\n", argv[0]);
    fprintf(stderr, "Or: %s -f pcap_file_path\n", argv[0]);
    return -1;
}
