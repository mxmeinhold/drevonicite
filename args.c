#include <stdio.h> // fprintf()
#include <unistd.h> // getopt(), access()
#include <string.h> // memset()
#include <errno.h> // errno and access() errors
#include <pcap/pcap.h>

#include "args.h"


int parse_args(int argc, char** argv, struct args* args) {
    // Reset all args to defaults
    memset(args, 0, sizeof(struct args));

    int opt;
    while ((opt = getopt(argc, argv, "pf:")) != -1) {
        switch ((char) opt) {
            case 'p':
                args->promiscuous = 1;
                break;
            case 'f':
                args->file = optarg;
                break;
            case '?':
                goto failure;
            case ':':
                goto failure;
        }
    }

    if (args->file) {
        if (access(args->file, R_OK)) {
            switch (errno) {
                case EACCES:
                    fprintf(stderr, "Unable to open file for reading: %s\n", args->file);
                    return -2;
                case ENOENT:
                    fprintf(stderr, "File does not exist: %s\n", args->file);
                    return -2;
                case ELOOP:
                    fprintf(stderr, "A loop exists in symbolic links encountered during resolution of the file argument.\n");
                    return -2;
                default:
                    return -2;
            }
        }
    } else {
        if (optind == argc) {
            // No data specified
            pcap_if_t* alldevs;
            pcap_findalldevs(&alldevs, NULL);

            if (alldevs != NULL) {
                fprintf(stderr,
                        "Please specify a file or one of the following interfaces:\n"
                       );
                while (alldevs != NULL) {
                    fprintf(stderr, "%s\n", alldevs->name);
                    alldevs = alldevs->next;
                }
            }
            fprintf(stderr, "\n");
            pcap_freealldevs(alldevs);
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
    fprintf(stderr, "Usage: %s [-p] interface_name\n", argv[0]);
    fprintf(stderr, "Or: %s [-p] -f pcap_file_path\n", argv[0]);
    return -1;
}
