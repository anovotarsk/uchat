#include "../inc/uchat.h"

static void *get_in_addr(struct sockaddr *sa) { 
    if (sa->sa_family == AF_INET) { 
        return &(((struct sockaddr_in*)sa)->sin_addr); 
    } 
    return &(((struct sockaddr_in6*)sa)->sin6_addr); 
}

static int do_accept(struct sockaddr_storage *their_addr, int sockfd) {
    int new_fd = 0;
    socklen_t sin_size;
    struct sockaddr_storage tmp;

    sin_size = sizeof *their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&tmp, &sin_size);
    *their_addr = tmp;
    if (new_fd == -1)
        perror("accept");
    return new_fd;
}



static void *send_recv_loop(void* new_fd) {
    char *recvest;
    char *login;
    long t = time(NULL);

    login = mx_login_reg(*(int*)new_fd);
    printf("user %s signin\n", login);
    printf("%s\n", online);
    //sleep(1);
    while (time(NULL) - t < 4);
    mx_send_message_first(*(int*)new_fd);
    //system("leaks -q uchat_server");
    while (1) {
        recvest = mx_recvester(*(int*)new_fd);
        printf("message - %s\n", recvest);
        mx_mailing_messages(recvest, login);
        mx_strdel(&recvest);
    }
}

void mx_accept_loop(int sockfd) {
    struct sockaddr_storage their_addr;
    int new_fd = 0;
    char s[INET6_ADDRSTRLEN];
    pthread_t thread;

    while (1) {
        new_fd = do_accept(&their_addr, sockfd);
        if (new_fd == -1)
            continue;
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
        pthread_create(&thread, NULL, send_recv_loop, (void*)&new_fd);
    }
}
