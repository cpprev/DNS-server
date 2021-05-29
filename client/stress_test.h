#ifndef STRESS_TESTER_H
#define STRESS_TESTER_H

#include "server/protocol.h"

#include "utils/string.h"

void basic_stress_test(PROTOCOL proto);

void thread_pool_stress_test(PROTOCOL proto);

#endif
