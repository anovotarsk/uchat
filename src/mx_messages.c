#include "../inc/uchat.h"

char *mx_new_message_id() {
    char *file = mx_file_to_str(MESSAGES);
    int count_messages = mx_count_words(file, '\v');
    char *new_id;

    if (count_messages == -1)
        count_messages = 0;
    new_id = mx_itoa(count_messages + 1);
    mx_strdel(&file);
    return new_id;
}

void mx_add_message(char *message, char *login) {
    char *str;
    char *tmp = "\t";
    char *buf;
    long t;
    int fd;

    str = mx_strcat(tmp, login); // \tlogin
    tmp = str;
    str = mx_strcat(tmp, "\t"); // \t login \t
    mx_strdel(&tmp);
    tmp = str;
    str = mx_strcat(tmp, message); // \t login \t message
    mx_strdel(&tmp);
    tmp = str;
    str = mx_strcat(tmp, "\t"); // \t login \t message \t
    mx_strdel(&tmp);
    tmp = str;
    t = time(NULL);
    buf = ctime(&t);
    tmp = mx_strnew(24);
    for (int i = 0; i < 24; i++)
        tmp[i] = buf[i];
    str = mx_strcat(str, tmp); // \t login \t message \t time
    mx_strdel(&tmp);
    tmp = str;
    buf = mx_new_message_id();
    str = mx_strcat(buf, tmp); // m_id \t login \t message \t time
    mx_strdel(&buf);
    mx_strdel(&tmp);
    fd = open(MESSAGES, O_WRONLY|O_APPEND);
    write(fd, str, strlen(str));
    write(fd, "\v", 1);
    close(fd);
    mx_strdel(&str);
}

void mx_send_message_first(int sockfd) {
    char *messages = mx_file_to_str(MESSAGES);

    if (messages != NULL)
        mx_sender(sockfd, messages);
}

void mx_mailing_messages(char *message, char *login) {
    char *messages;
    char **fd_mas;
    int i;

    mx_add_message(message, login);
    messages = mx_file_to_str(MESSAGES);
    fd_mas = mx_strsplit(online, '\n');
    mx_print_strarr(fd_mas, " ");
    for (i = 0; fd_mas[i] != NULL; i++) {
        mx_sender(atoi(fd_mas[i]), messages);
    }
    mx_printstr("Sended\n");
    mx_strdel(&messages);
    mx_del_strarr(&fd_mas);
}
