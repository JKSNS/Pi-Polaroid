#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "log.h"

int client_connect(const Config *config) {
    // Sets up structs for address info of specified hostname/port pair
    log_info("Connecting to server...");
    struct addrinfo hints, *result, *rp;
    int sockfd = -1;
    int status;

    // Sets the IP version to unspecified to be version neutral and the socket type to TCP
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Checks to see if there is correct address info for hostname/port pair
    if ((status = getaddrinfo(config->host, config->port, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Loops through linked list pairs of info to establish a valid socket
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        // Socket incorrect
        if (sockfd < 0) {
            printf(strerror(ENOTCONN));
            continue;
        }

        // Checks to see if connection is valid
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            log_info("Connection established");
            break;
        } else {
            printf(strerror(ETIMEDOUT));
            close(sockfd);
        }
    }

    freeaddrinfo(result); // free the linked list
    if (sockfd == -1) {
        printf("could not establish connection with TCP server\n");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
/*
void client_send_image(int sockfd, const Config *config) {
    // IMPLEMENT THIS FUNCTION
    // send hw_id
    send(sockfd, config->hw_id, strlen(config->hw_id), 0);
    // send the image payload
    int bytes_sent = 0;
    while (bytes_sent < config->payload_size) {
        int n = send(sockfd, config->payload + bytes_sent, config->payload_size - bytes_sent, 0);
        if (n == -1) {
            perror("send");
            break;
        }
        bytes_sent += n;
    }
}
*/

void client_send_image(int sockfd, const Config *config) {
    size_t hw_id_len = strlen(config->hw_id);
    size_t total_size = hw_id_len + config->payload_size;
    uint8_t *data_to_send = malloc(total_size);

    if (data_to_send == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Copy hw_id and payload into data_to_send
    memcpy(data_to_send, config->hw_id, hw_id_len);
    memcpy(data_to_send + hw_id_len, config->payload, config->payload_size);

    ssize_t total_sent = 0;
    while (total_sent < total_size) {
        ssize_t num_sent = send(sockfd, data_to_send + total_sent, total_size - total_sent, 0);
        if (num_sent < 0) {
            perror("send");
            free(data_to_send);
            exit(EXIT_FAILURE);
        }
        total_sent += num_sent;
    }

    free(data_to_send);
}

void client_receive_response(int sockfd) {
    uint8_t buf[101];
    int num_recv = recv(sockfd, buf, 100, 0);
    buf[num_recv] = '\0';

    printf("Response from server: %s\n", buf);
}

void client_close(int sockfd) {
    // IMPLEMENT THIS FUNCTION
    close(sockfd);
}