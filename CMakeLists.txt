cmake_minimum_required(VERSION 3.10)

project(dns VERSION 1.0 LANGUAGES C)

include_directories(src)

set(SRC
    # Config
    src/config/server_config.c
    src/config/zone_array.c
    src/config/zone.c

    # Utils
    src/utils/string.c
    src/utils/error.c
    src/utils/file.c
    src/utils/json.c
)

set(CHECK_CFLAGS -fsanitize=address -g)

add_compile_options(${CHECK_CFLAGS} -Wall -Wextra -Werror -pedantic -std=c99)

add_executable(dns src/main.c ${SRC})

target_link_libraries(dns asan)
