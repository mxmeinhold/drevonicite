#include <stdio.h> // printf()

#include "args.h"


int main(int argc, char** argv) {

    struct args args;

    if (parse_args(argc, argv, &args) != 0) {
        return -1; // TODO document error codes
    }

    printf("Args: {\n\tfile: %s\n\tiface: %s\n}\n", args.file, args.interface);

    return 0;
}
