#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils/error.h"

#include "connect.h"
#include "client_options.h"

#define RECV_SIZE 256

int g_nb_req = 0;
client_options *g_options = NULL;

int tcp_send_request()
{
    struct sockaddr_in sin;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    exit_if_true(sock < 0, "socket error");
    memset ((char *) &sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(g_options->port);
	inet_pton(AF_INET, g_options->ip->arr, &sin.sin_addr);

    exit_if_true(connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0, "connect error");

    if (send(sock, g_options->message, g_options->message_size, 0) >= 0)
    {
        char buf[RECV_SIZE + 1];
        int buf_len = recv(sock, buf, RECV_SIZE, 0);
        if (buf_len > 0)
        {
            buf[buf_len] = '\0';
            g_nb_req += 1;
        }
    }
    return 0;
}

void *udp_send_request()
{
    struct sockaddr_in sin;
    int sin_len = sizeof(sin);
    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    exit_if_true(sock < 0, "socket error");
    memset ((char *) &sin, 0, sizeof (sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(g_options->port);
    inet_pton(AF_INET, g_options->ip->arr, &sin.sin_addr);

    if (sendto(sock, g_options->message, g_options->message_size, 0, (struct sockaddr *) &sin, sin_len) != -1)
    {
        char buf[RECV_SIZE + 1];
        int buf_len = recvfrom(sock, buf, RECV_SIZE, 0, (struct sockaddr *)&sin, (socklen_t *)&sin_len);
        if (buf_len > 0)
        {
            buf[buf_len] = '\0';
            g_nb_req += 1;
        }
    }
    return NULL;
}
