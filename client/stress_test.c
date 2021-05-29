#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "stress_test.h"
#include "connect.h"

#define THREAD_CAP 16

void *udp_routine()
{
    while (true)
    {
        udp_send_request();
    }
}

void *tcp_routine()
{
    while (true)
    {
        tcp_send_request();
    }
}

void thread_pool_stress_test(PROTOCOL proto)
{
    pthread_t tid[THREAD_CAP + 1];
    int i = 0;
    while (i < THREAD_CAP)
    {
        int p = 0;
        if (proto == UDP)
            p = pthread_create(&tid[i++], NULL, udp_routine, NULL);
        else
            p = pthread_create(&tid[i++], NULL, tcp_routine, NULL);
        if (p != 0 || errno == EAGAIN)
        {
            --i;
            break;
        }
    }

    while (true)
        printf("NB requests sent: %d\n", g_nb_req);
}

void basic_stress_test(PROTOCOL proto)
{
    while (1)
    {
        if (proto == UDP)
            udp_send_request(g_message, g_ip, g_port);
        else if (proto == TCP)
            tcp_send_request(g_message, g_ip, g_port);
        printf("NB requests sent: %d\n", g_nb_req);
    }
}
