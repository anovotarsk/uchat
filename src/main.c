#include "../inc/uchat.h"

char *online;

int main() {
    int sockfd;

    online = mx_strnew(1);
    //mx_add_message("nuirnuinr", "anovotarsk");
    sockfd = mx_creating_server();
    mx_accept_loop(sockfd);
}
