#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils/error.h"

#include "connect.h"

#define BUF_SIZE 1024

void tcp_send_request(string *message, string *ip, int port)
{
    struct sockaddr_in sin;
    int sock;
    //char buf[BUF_SIZE + 1];
    //int buf_len;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    exit_if_true(sock < 0, "socket error");
    memset ((char *) &sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
	inet_pton(AF_INET, (const char*)ip, &sin.sin_addr);

    exit_if_true(connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0, "connect error");

    if (send(sock, message->arr, message->size, 0) >= 0)
    {
        /*buf_len = recv(sock, buf, BUF_SIZE, 0);
        buf[buf_len] = '\0';
        printf("response from server: %s\n", buf);*/
    }
}

void udp_send_request(string *message, string *ip, int port)
{
    struct sockaddr_in sin;
    int sin_len = sizeof(sin);
    int sock;
    //char buf[BUF_SIZE + 1];
    //int buf_len = 0;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    exit_if_true(sock < 0, "socket error");
    memset ((char *) &sin, 0, sizeof (sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, (const char*)ip, &sin.sin_addr);

    if (sendto(sock, message->arr, message->size, 0, (struct sockaddr *) &sin, sin_len) >= 0)
    {
        /*buf_len = recv(sock, buf, BUF_SIZE, 0);
        buf[buf_len] = '\0';
        printf ("response from server: %s\n", buf);*/
    }
}
