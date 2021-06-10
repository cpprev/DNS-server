cmake_minimum_required(VERSION 3.10)

project(dns VERSION 1.0 LANGUAGES C)

include_directories(src/)

set(SRC
    # Config
    src/config/server_config.c
    src/config/zone_array.c
    src/config/zone.c
    src/config/record_array.c
    src/config/record.c
    src/config/record_type.c
    src/config/class_type.c
    src/config/record_values.c

    # Messages
    src/messages/message.c
    src/messages/rcode.c
    src/messages/qr.c
    src/messages/opcode.c
    src/messages/request/request.c
    src/messages/response/response.c
    src/messages/response/resp_headers.c
    src/messages/response/resp_question.c
    src/messages/response/resp_answer.c
    src/messages/question.c
    src/messages/question_array.c

    # Parsing
    src/parser/input.c
    src/parser/parse_request.c
    src/parser/parse_zone.c
    src/parser/options.c

    # Server
    src/server/udp_listen.c
    src/server/tcp_listen.c
    src/server/wrapper.c

    # Utils
    src/utils/utils.c
    src/utils/string.c
    src/utils/bits.c
    src/utils/error.c
    src/utils/file.c
    src/utils/base_convertions.c
    src/utils/printer.c

    # Client
    src/client/connect.c
    src/client/build_req.c
    src/client/stress_test.c
    src/client/client_options.c
)

set(DEBUG_CFLAGS -fsanitize=address -g)
set(DEFAULT_C_FLAGS -Wall -Wextra -Werror -pedantic -std=c99)

add_compile_options(${DEFAULT_C_FLAGS})

add_executable(dns src/main.c ${SRC})
add_executable(client src/client/main.c ${SRC})
add_executable(profiler src/profiler/main.c ${SRC})

target_link_libraries(dns pthread)
target_link_libraries(client asan pthread)
target_link_libraries(profiler pthread)

add_custom_target(check COMMAND make -j5 && cp dns ../tests && cd "${CMAKE_SOURCE_DIR}/tests" && ./main.sh)
