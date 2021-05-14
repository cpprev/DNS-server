cmake_minimum_required(VERSION 3.10)

project(dns VERSION 1.0 LANGUAGES C)

include_directories(src)

set(SRC
    # Config
    src/config/server_config.c
    src/config/zone_array.c
    src/config/zone.c
    src/config/record_array.c
    src/config/record.c
    src/config/record_type.c

    # Parsing
    src/parsing/input.c
    src/parsing/request.c

    # Server
    src/server/listen.c

    # Utils
    src/utils/string.c
    src/utils/error.c
    src/utils/file.c
    src/utils/base_convertions.c
)

set(CHECK_CFLAGS -fsanitize=address -g)

add_compile_options(${CHECK_CFLAGS} -Wall -Wextra -Werror -pedantic -std=c99)

add_executable(dns src/main.c ${SRC})

target_link_libraries(dns asan)
