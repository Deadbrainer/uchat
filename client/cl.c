#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../libmx/inc/libmx.h"

// char msg[500];

void get_array(char** array, int check) { // to an array from main
    static char *n;

    if(check) {
        n = *array;
    } else {
        *array = n;
    }
}

void *recvmg(void *my_sock) {
    char* msg;
    get_array(&msg, 0);

    int sock = *((int *)my_sock);
    int len;
    while ((len = recv(sock, msg, 500, 0)) > 0) {
        msg[len] = '\0';
        fputs(msg, stdout);
    }
    return NULL; // to silence warning
} // should not return anything

void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Wrong usage\n");
        return 0;
    }

    int port = mx_atoi(argv[1]);

    //char msg[500];
    char* msg = malloc(500);

    pthread_t recvt;
    int len;
    int sock;

    char* send_msg = malloc(500);

    struct sockaddr_in ServerIp;


    // strcpy(client_name, argv[1]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(port);
    ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock, (struct sockaddr*)&ServerIp, sizeof(ServerIp)) == -1) {
        fprintf(stderr, "Connection failure\n");
    } else {
        printf("Connected\n");
    }

    get_array(&msg, 1); // assign value

    pthread_create(&recvt, NULL, (void*)recvmg, &sock); // client thread which is always waiting for a message

    char client_name[32];

    printf("Please enter your name: ");
  	fgets(client_name, 32, stdin);
  	str_trim_lf(client_name, strlen(client_name));

    char client_password[32];

    printf("Please enter your passsword: ");
  	fgets(client_password, 32, stdin);
  	str_trim_lf(client_password, strlen(client_password));

    char* to_send = mx_strjoin(client_name, " ");
    to_send = mx_strjoin(to_send, client_password);

    send(sock, to_send, mx_strlen(to_send), 0); // MY OWN


    //read a message from stdin (console)
    while(fgets(msg, 500, stdin) > 0) {
        strcpy(send_msg, client_name);
        strcat(send_msg, ":  ");
        strcat(send_msg, msg);
        len = write(sock, send_msg, strlen(send_msg));
        if (len < 0) {
            printf("\n message not send");
        }
    }

    //thread is closed
    pthread_join(recvt, NULL);
    close(sock);
    return 0;
}
