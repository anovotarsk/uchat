#include "../inc/uchat.h"

int mx_creating_server() {
    int sockfd = 0;
    struct addrinfo *servinfo = NULL;

	mx_check_addrinfo(&servinfo);
	mx_trybind(&servinfo, &sockfd);
    // freeaddrinfo(servinfo);
	mx_trysigaction(&sockfd);
    return sockfd;
}
