#ifndef DREVONICITE_ARGS_H
#define DREVONICITE_ARGS_H

struct args {
    // The name of the pcap file to read from
    char* file;
    // The name of the interface to read from
    char* interface;
};

/**
 * Parse commandline arguments into a struct args
 * Arguments:
 * - argc: the number of entries in argv (from main)
 * - argv: the arguments list (from main)
 * - args: the args struct to fill
 * Return:
 * -  0: success
 * - -1: Bad arguments given
 */
int parse_args(int argc, char** argv, struct args* args);

#endif
