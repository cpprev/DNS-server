#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "stress_test.h"
#include "connect.h"

#define THREAD_CAP 6

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

void thread_pool_stress_test(PROTOCOL proto)
{
    (void) proto;

    pthread_t tid[THREAD_CAP + 1];
    int i = 0;

    while (true)
    {
        int p = pthread_create(&tid[i++], NULL, udp_send_request, NULL);
        printf("%d\n", p);
        if (p != 0)
            perror("thread create error, reduce number of threads.");
        if (i >= THREAD_CAP - 1)
        {
            i = 0;
            while (i < THREAD_CAP - 1)
            {
                if (pthread_join(tid[i++],NULL) != 0)
                    perror("thread join error, reduce number of threads.");
            }
        }
        printf("NB requests sent: %d\n", g_nb_req);
    }
}
