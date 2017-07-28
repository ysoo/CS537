#include "server_impl.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>

void server_init(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s server [portnum] [threads] [buffers]\n", argv[0]);
        exit(1);
    }
}

void server_dispatch(int connfd) {
    // Handle the request in the main thread.
    requestHandle(connfd);
}
